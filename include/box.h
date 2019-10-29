// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <opt.h>
#include <safe.h>
#include <mnd.h>

namespace cat {

template <typename T>
using box = opt<safe<T>>;

template <typename T>
class any<safe<T>, nil>
{
public:
    class iter
    {
    public:
        const safe<T>& operator*() const { return *p; }
        bool operator!=(nil) const { return p != nullptr; }
        bool operator==(nil) const { return p == nullptr; }
        void operator++() { p = nullptr; }

    private:
        iter(const safe<T>* p) : p(p) {}
        const safe<T>* p;
        friend class any;
    };

    constexpr any() : p(nil{}) {}
    constexpr any(nil n) : p(n) {}
    any& operator << (nil n) { p = safe<T>{n}; }

    any(const any &t) = delete;
    any& operator=(const any &t) = delete;

    template <typename U>
    any(safe<U>&& u) : p(std::move(u)) {}
    template <typename U>
    any& operator << (safe<U>&& u) { p = std::move(u); }

    constexpr std::size_t index() const { return p.operator->() ? 0 : 1; }

    template <typename F>
    join_t<opt, F, const safe<T>&> operator >>= (F f) const
    {
        if constexpr (std::is_void_v<join_t<opt, F, const safe<T>&>>) {
            if (p.operator->()) {
                f(p);
            }
        } else {
            if (p.operator->())
                return f(p);
            else
                return {};
        }
    }

    iter begin() const { return iter{p.operator->() ? &p : nullptr}; }
    nil end() const { return {}; }

private:
    safe<T> p;
};

}
