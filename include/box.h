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
class opt<safe<T>>
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
        friend class opt;
    };

    constexpr opt() : p(nil{}) {}
    constexpr opt(nil n) : p(n) {}
    opt& operator << (nil n) { p = safe<T>{n}; return *this; }

    opt(opt &&t) = default;
    opt& operator=(opt &&t) = default;

    opt(const opt &t) = delete;
    opt& operator=(const opt &t) = delete;

    template <typename U>
    opt(safe<U>&& u) : p(std::move(u)) {}
    template <typename U>
    opt(std::in_place_t, safe<U>&& u) : p(std::move(u)) {}
    template <typename U>
    opt& operator << (safe<U>&& u) { p = std::move(u); }

    constexpr std::size_t index() const { return p.operator->() ? 0 : 1; }

    template <typename F>
    decltype (auto) operator >>= (F f) const
    {
        using opt_t = join_t<opt, F, const safe<T>&>;
        if constexpr (std::is_void_v<join_t<opt, F, const safe<T>&>>) {
            if (p.operator->()) {
                f(p);
            }
        } else {
            if (p.operator->())
                return opt_t{std::in_place, f(p)};
            else
                return opt_t{};
        }
    }

    iter begin() const { return iter{p.operator->() ? &p : nullptr}; }
    nil end() const { return {}; }

private:
    safe<T> p;
};

}
