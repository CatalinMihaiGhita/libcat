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
class lazy;

namespace impl {

class abstract_cell
{
public:
    virtual void take(void*) {}
    virtual ~abstract_cell() {}
};

template <class T>
class cell : public abstract_cell
{
public:
    void push(T&& t)
    {
        val = std::move(t);
        if (next)
            next->take(val.operator->());
    }

    void push(const T& t)
    {
        val = t;
        if (next)
            next->take(val.operator->());
    }

    void push(lazy<T>&& t) const
    {
        t.p->join(next);
        t.abort();
    }

    void push(const lazy<T>& t) const
    {
        t.p->join(next);
    }

    void join(std::shared_ptr<abstract_cell> other)
    {
        if (other) {
            next = std::move(other);
            if (val) {
                next->take(val.operator->());
            }
        }
    }

    void take(void* t) override
    {
        push(*static_cast<T*>(t));
    }

    std::optional<T> val;
    std::shared_ptr<abstract_cell> next;
};


template <>
class cell<void> : public abstract_cell
{
};

template <class F, class T>
class susp_cell : public cell<flatten_t<lazy, std::invoke_result_t<F,T>>>
{
public:
    susp_cell(F &&f)
        : f(std::forward<F>(f))
    {
    }

    void take(void* t) override
    {
        if constexpr (std::is_void_v<flatten_t<lazy, std::invoke_result_t<F,T>>>) {
            f(*static_cast<T*>(t));
        } else {
            this->push(f(*static_cast<T*>(t)));
        }
    }

private:
    F f;
};

}

template <class T>
class lazy
{
    lazy(std::shared_ptr<impl::cell<T>> p) : p(std::move(p)) {}

public:
    lazy() : p(std::make_shared<impl::cell<T>>()) {}

    lazy(T&& t)
        : p(std::make_shared<impl::cell<T>>())
    {
        p->val = std::move(t);
    }

    lazy(const T& t)
        : p(std::make_shared<impl::cell<T>>())
    {
        p->val = t;
    }

    lazy(const lazy &t) = delete;
    lazy& operator=(const lazy &t) = delete;

    lazy(lazy &&t) = default;
    lazy& operator=(lazy &&t) = default;

    lazy& operator << (T&& t)
    {
        p->push(std::move(t));
        return *this;
    }

    lazy& operator << (const T& t)
    {
        p->push(t);
        return *this;
    }

    lazy& abort()
    {
        p = std::make_shared<impl::cell<T>>();
        return *this;
    }

    lazy& operator << (const lazy<T>& other)
    {
        p->push(other);
        return *this;
    }

    lazy& operator << (lazy<T>&& other)
    {
        p->push(std::move(other));
        return *this;
    }

    template <class F>
    decltype (auto) operator >>= (F&& f) const
    {
        using lazy_t = join_t<lazy, F, const T&>;
        if constexpr (std::is_void_v<lazy_t>) {
            p->join(std::make_shared<impl::susp_cell<F, T>>(std::forward<F>(f)));
        } else {
            auto scell = std::make_shared<impl::susp_cell<F, T>>(std::forward<F>(f));
            p->join(scell);
            return lazy_t{std::move(scell)};
        }
    }

    lazy operator++() const { return p; }

private:
    friend class impl::cell<T>;
    template <typename U>
    friend class lazy;
    std::shared_ptr<impl::cell<T>> p;
};

template <typename T, typename... U>
lazy<T> wrap_lazy(U&&... t)
{
    return {std::forward<U>(t)...};
}

template <class T>
struct flatten<lazy, lazy<T>>
{
    typedef T type;
};

template <typename T>
class is_monad<lazy<T>> : public std::true_type
{
};

}
