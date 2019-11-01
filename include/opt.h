// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <optional>

#include <any.h>
#include <all.h>
#include <safe.h>
#include <mnd.h>

namespace cat {

template <typename T>
class opt
{
public:
    class iter
    {
    public:
        const T& operator*() const { return **p; }
        bool operator!=(nil) const { return p != nullptr; }
        bool operator==(nil) const { return p == nullptr; }
        void operator++() { p = nullptr; }
    private:
        iter(const std::optional<T>* p) : p(p) {}
        const std::optional<T>* p;
        friend class opt;
    };

    constexpr opt() {}
    constexpr opt(nil) {}
    opt& operator << (nil) { p.reset(); return *this; }

    template <typename... U>
    opt(std::in_place_t, U&&... u) : p{std::in_place, std::forward<U>(u)...} {}
    template <typename U>
    opt& operator << (U&& u) { p = std::forward(u); return *this; }

    constexpr std::size_t index() const { return p.operator->() ? 0 : 1; }

    template <typename F>
    decltype (auto) operator >>= (F f) const
    {
        using opt_t = join_t<opt, F, const T&>;
        if constexpr (std::is_void_v<opt_t>) {
            if (p) f(*p);
        } else {
            if (p)
                return opt_t{std::in_place, f(*p)};
            else
                return opt_t{};
        }
    }

    iter begin() const { return iter{p ? &p : nullptr}; }
    nil end() const { return {}; }

private:
    std::optional<T> p;
};

template <typename T, typename... U>
opt<T> wrap_opt(U&&... t)
{
    return {std::in_place, std::forward<U>(t)...};
}

template <typename T>
class is_monad<opt<T>> : public std::true_type
{
};

}
