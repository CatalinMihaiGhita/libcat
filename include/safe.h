// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <memory>
#include <all.h>

namespace cat {

template <typename T>
class safe
{
    template <typename U>
    friend class opt;

    constexpr safe(nil) {}
public:

    template <typename... U>
    explicit safe(std::in_place_t, U&&... t) : p(new T{std::forward<U>(t)...}) {}

    template <typename U = T>
    safe(safe<U>&& u) : p(std::move(u.p)) {}
    template <typename U = T>
    safe& operator=(safe<U> &&t) { p = std::move(t.p); }

    T* operator->() const noexcept { return p.get(); }
    std::add_lvalue_reference_t<T> operator*() const { return *p; }

private:
    template <class U>
    friend class safe;
    std::unique_ptr<T> p;
};

template <typename T, typename... U>
safe<T> wrap_safe(U&&... t)
{
    return safe<T>{std::in_place, std::forward<U>(t)...};
}

}
