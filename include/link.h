// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <all.h>

namespace cat {

namespace impl {
    struct node
    {
        node()
            : prev(this)
            , next(this)
        {
        }

        void insert(const node& o)
        {
            prev = &o;
            next = o.next;
            next->prev = o.next = this;
        }

        bool erase()
        {
            if (next != this) {
                next->prev = prev;
                prev->next = next;
                return false;
            }
            return true;
        }

        mutable const node* prev;
        mutable const node* next;
    };
}

template <class T>
class link
{
    template<class U>
    friend class link;

    T* p;
    impl::node n;

public:
	template<class... Args>
    link(std::in_place_t, Args&&... args)
		: p {new T{std::forward<Args>(args)...}}
	{
	}

    link(const link& o) noexcept
    {
        n.insert(o.n);
        p = o.p;
    }

    link& operator=(const link& o) noexcept
    {
        ~link();
        n.insert(o.n);
        p = o.p;
        return *this;
    }

    T* operator->() const noexcept { return p; }
    std::add_lvalue_reference_t<T> operator*() const { return *p; }

    ~link()
    {
        if (n.erase()) {
            delete p;
        }
    }
};

template <typename T, typename... U>
link<T> wrap_link(U&&... t)
{
    return {std::in_place, std::forward<U>(t)...};
}

}
