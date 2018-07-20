// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <memory>
#include <all.h>

namespace cat {

template <typename T>
class rc
{
    template <typename... U>
    friend class any;

    constexpr rc(nil) {}

    template <typename U = T>
    rc(const rc<U> &t) : p(t.p) {}

public:
    template <typename U = T>
    rc(rc<U> &&t) : p(std::move(t.p)) {}

    template <typename... U>
    rc(std::in_place_t, U&&... t) : p(new T(std::forward<U>(t)...)) {}
    T* operator->() const noexcept { return p.get(); }

    std::add_lvalue_reference_t<T> operator*() const { return *p; }

    rc<T> operator++() const {
        return rc<T>(*this);
    }

private:
    template <typename U>
    friend class rc;

    std::shared_ptr<T> p;
};

template<class T>
rc(T) -> rc<T>;

}
