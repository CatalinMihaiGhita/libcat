// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <mnd.h>

#include <rc.h>
#include <opt.h>
#include <any.h>

namespace cat {

template <class T>
using lzy = any<T, nvr>;

template <class T>
struct flatten<lzy, lzy<T>>
{
    typedef T type;
};

template <typename T>
class is_monad<lzy<T>> : public std::true_type
{
};

namespace impl {

class abstract_cell
{
public:
    virtual void take(void* t) = 0;
    virtual ~abstract_cell() {}
};

template <class T>
class cell : public abstract_cell
{
public:
    void push(T&& t)
    {
        if (next)
            next->take(&t);
        else
            val = std::move(t);
    }

    void push(lzy<T>&& t) const
    {
        t.p->join(next);
    }

    void push(const lzy<T>& t) const
    {
        t.p->join(next);
    }

    void join(const std::shared_ptr<abstract_cell>& other)
    {
        if (other) {
            next = other;
            if (val) {
                next->take(val.operator->());
                val.reset();
            }
        }
    }

    void take(void* t) override
    {
        push(std::move(*static_cast<T*>(t)));
    }

    std::optional<T> val;
    std::shared_ptr<abstract_cell> next;
};



template <class F, class T>
class susp_cell : public cell<flatten_t<lzy, std::invoke_result_t<F,T>>>
{
public:
    susp_cell(F &&f)
        : f(std::forward<F>(f))
    {
    }

    void take(void* t) override
    {
        this->push(f(std::move(*static_cast<T*>(t))));
    }

private:
    F f;
};

}

template <class T>
class any<T, nvr>
{
    any(std::shared_ptr<impl::cell<T>> p) : p(std::move(p)) {}

public:
    any() : p(std::make_shared<impl::cell<T>>()) {}
    any(std::in_place_t, T t)
        : p(std::make_shared<impl::cell<T>>())
    {
        p->val = std::move(t);
    }

    any(const any &t) = delete;
    any& operator=(const any &t) = delete;

    any& operator << (T t) {
        p->push(std::move(t));
        return *this;
    }

    any& operator << (const lzy<T>& other)
    {
        p->push(other);
        return *this;
    }

    any& operator << (lzy<T>&& other)
    {
        p->push(std::move(other));
        return *this;
    }

    template <class F>
    join_t<lzy, F, T> operator >>= (F&& f) const
    {
        auto scell = std::make_shared<impl::susp_cell<F, T>>(std::forward<F>(f));
        p->join(scell);
        return join_t<lzy, F, T>(std::move(scell));
    }

    any& operator++() const { return p; }

private:
    friend class impl::cell<T>;
    template <class... U>
    friend class any;
    std::shared_ptr<impl::cell<T>> p;
};

template <typename T, typename... U>
lzy<T> wrap_lzy(U&&... t)
{
    return {std::in_place, std::forward<U>(t)...};
}

}
