// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <monad.h>

#include <rc.h>
#include <opt.h>
#include <any.h>

namespace cat {

template <class T>
using lazy = any<T, nvr>;

namespace kernel {

class abstract_action
{
public:
    virtual void exec(void* t) = 0;
    virtual ~abstract_action() {};
};

template <class T>
class action : public abstract_action
{
public:
    void exec_next(T& t)
    {
        if (next)
            next->exec(&t);
        else
            val = std::move(t);
    }

    void exec_next(lazy<T>& t) const
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

    std::shared_ptr<abstract_action> next;
    std::optional<T> val;
};



template <class F, class T>
class join_action : public action<flatten_t<lazy, std::invoke_result_t<F,T>>>
{
public:
    join_action(F &&f)
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
class any<T, nvr> : monad
{
    any(std::shared_ptr<kernel::action<T>> p) : p(std::move(p)) {}

public:
    any()
        : p(std::make_shared<kernel::action<T>>())
    {}

    any(std::in_place_t, T t)
        : p(std::make_shared<kernel::action<T>>())
    {
        p->val = std::move(t);
    }

    void operator<<(T t) const {
        p->exec_next(t);
    }

    void operator<<(const lazy<T>& other) const
    {
        other.p->next = p;
    }

    template <class F>
    join_t<lazy, F, T> operator >>= (F&& f) const
    {
        auto action = std::make_shared<kernel::join_action<F, T>>(std::forward<F>(f));
        p->next = action;
        if (p->val) {
            p->next->exec(p->val.operator->());
            p->val.reset();
        }
        return join_t<lazy, F, T>(std::move(action));
    }

    ~any()
    {
    }

private:
    friend class kernel::abstract_action;
    template <class... U>
    friend class any;
    std::shared_ptr<kernel::action<T>> p;
};

}
