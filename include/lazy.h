// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <monad.h>

#include <rc.h>
#include <opt.h>

namespace cat {

template <class T>
class lazy;

namespace kernel {

class action
{
public:
    template <class T>
    void exec_next(T& t) const
    {
        if (next)
            next->exec(&t);
    }

    template <class T>
    void exec_next(lazy<T>& t) const
    {
        t.p->next = next;
    }

    virtual void exec(void* t)
    {
        if (next)
            next->exec(t);
    }

    virtual ~action()
    {
    }

    std::shared_ptr<action> next;
};



template <class F, class T>
class join_action : public action
{
public:
    join_action(F &&f)
        : f(std::forward<F>(f))
    {
    }

    void exec(void* t) override
    {
        auto rez = f(std::move(*static_cast<T*>(t)));
        exec_next(rez);
    }

private:
    F f;
};

}

template <class T>
class lazy : monad
{
    lazy(std::shared_ptr<kernel::action> p) : p(std::move(p)) {}

public:
    lazy()
        : p(std::make_shared<kernel::action>())
    {}

    void operator<<(T t) const {
        p->exec_next(t);
    }

    void operator<<(const lazy<T>& other) const
    {
        other.p->next = p;
    }

    template <class F>
    join<lazy, F, T> operator >>= (F&& f) const
    {
        join<lazy, F, T> r(std::make_shared<kernel::join_action<F, T>>(std::forward<F>(f)));
        p->next = r.p;
        return r;
    }

    ~lazy()
    {
    }

private:
    friend class kernel::action;
    template <class U>
    friend class lazy;
    std::shared_ptr<kernel::action> p;
};

}
