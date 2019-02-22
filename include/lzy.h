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

template <typename T>
class is_monad<lzy<T>> : public std::true_type
{
};

namespace impl {

class itask
{
public:
    virtual void exec(void* t) = 0;
    virtual ~itask() {};
};

template <class T>
class task : public itask
{
public:
    void exec_next(T& t)
    {
        if (next)
            next->exec(&t);
        else
            val = std::move(t);
    }

    void exec_next(lzy<T>& t) const
    {
        t.p->next = next;
    }

    void exec(void* t) override
    {
        if (next)
            next->exec(t);
        else
            val = std::move(*static_cast<T*>(t));
    }

    std::shared_ptr<itask> next;
    std::optional<T> val;
};



template <class F, class T>
class ftask : public task<flatten_t<lzy, std::invoke_result_t<F,T>>>
{
public:
    ftask(F &&f)
        : f(std::forward<F>(f))
    {
    }

    void exec(void* t) override
    {
        auto rez = f(std::move(*static_cast<T*>(t)));
        this->exec_next(rez);
    }

private:
    F f;
};

}

template <class T>
class any<T, nvr>
{
    any(std::shared_ptr<impl::task<T>> p) : p(std::move(p)) {}

public:
    any() : p(std::make_shared<impl::task<T>>()) {}
    any(std::in_place_t, T t)
        : p(std::make_shared<impl::task<T>>())
    {
        p->val = std::move(t);
    }

    any(const any &t) = delete;
    any& operator=(const any &t) = delete;

    any& operator << (T t) {
        p->exec_next(t);
        return *this;
    }

    any& operator << (const lzy<T>& other)
    {
        other.p->next = p;
        if (other.p->val) {
            p->exec_next(*other.p->val);
            other.p->val.reset();
        }
        return *this;
    }

    template <class F>
    join_t<lzy, F, T> operator >>= (F&& f) const
    {
        auto task = std::make_shared<impl::ftask<F, T>>(std::forward<F>(f));
        p->next = task;
        if (p->val) {
            p->next->exec(p->val.operator->());
            p->val.reset();
        }
        return join_t<lzy, F, T>(std::move(task));
    }

    any& operator++() const { return p; }

private:
    friend class impl::itask;
    template <class... U>
    friend class any;
    std::shared_ptr<impl::task<T>> p;
};

}
