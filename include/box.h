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
    template <typename... U>
    box(U&&... t) : p(new T(std::forward<U>(t)...)) {}
    T* operator->() const noexcept { return p.get(); }
    std::add_lvalue_reference_t<T> operator*() const { return *p; }
private:
    std::unique_ptr<T> p;
};

template<class T>
box(T) -> box<T>;

}
