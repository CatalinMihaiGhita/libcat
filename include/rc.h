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
    template <typename U>
    friend class opt;

    rc(std::shared_ptr<T> t) : p(std::move(t)) {}
    constexpr rc(nil) {}
    void reset() { p.reset(); }

public:
    template <typename... U>
    explicit rc(std::in_place_t, U&&... t) : p(std::make_shared<T>(std::forward<U>(t)...)) {}

    rc(const rc<T> &) = delete;
    rc& operator=(const rc<T> &) = delete;

    template <typename U = T>
    rc(rc<U> &&t) : p(std::move(t.p)) {}
    template <typename U = T>
    rc& operator=(rc<U> &&t) { p = std::move(t.p); }

    T* operator->() const noexcept { return p.get(); }
    std::add_lvalue_reference_t<T> operator*() const { return *p; }

    rc<T> operator++() const { return p; }

private:
    template <typename U>
    friend class rc;

    std::shared_ptr<T> p;
};

template <typename T, typename... U>
rc<T> wrap_rc(U&&... t)
{
    return rc<T>{std::in_place, std::forward<U>(t)...};
}

}
