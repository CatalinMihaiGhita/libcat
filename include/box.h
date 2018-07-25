// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <memory>
#include <all.h>

namespace cat {

template <typename T>
class box
{
    template <typename... U>
    friend class any;

    constexpr box(nil) {}
public:
    template <typename U>
    box(box<U>&& u) : p(std::move(u.p)) {}
    template <typename... U>
    box(std::in_place_t, U&&... t) : p(new T(std::forward<U>(t)...)) {}

    T* operator->() const noexcept { return p.get(); }
    std::add_lvalue_reference_t<T> operator*() const { return *p; }

private:
    template <class U>
    friend class box;
    std::unique_ptr<T> p;
};

template<class T>
box(T) -> box<T>;

template <typename T, typename... U>
box<T> wrap_box(U&&... t)
{
    return box<T>{std::in_place, std::forward<U>(t)...};
}

}
