// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <optional>

#include <var.h>
#include <tup.h>
#include <mnd.h>

namespace cat {

template <typename T>
class opt
{
public:
    using flat_type = T;

    class iter
    {
    public:
        const T& operator*() const { return **p; }
        bool operator!=(unit) const { return p != nullptr; }
        bool operator==(unit) const { return p == nullptr; }
        void operator++() { p = nullptr; }
    private:
        iter(const std::optional<T>* p) : p(p) {}
        const std::optional<T>* p;
        friend class opt;
    };

    constexpr opt() {}
    constexpr opt(unit) {}

    template <typename U = T>
    constexpr opt(U&& u) : p{std::forward<U>(u)} {}

    template <typename... U>
    explicit opt(std::in_place_t, U&&... u) : p{std::in_place, std::forward<U>(u)...} {}

    template <typename U = T>
    opt& operator << (U&& u) { p = std::forward<U>(u); return *this; }
    template <typename U>
    opt& operator << (const opt<U>& u) { p = u.p; return *this; }
    template <typename U>
    opt& operator << (opt<U>&& u) { p = std::move(u.p); return *this; }
    opt& operator << (unit) { p.reset(); return *this; }

    constexpr std::size_t index() const { return p.operator->() ? 0 : 1; }

    template <typename F>
    decltype (auto) operator >>= (F f) const &
    {
        using opt_t = join_t<opt, F, const T&>;
        if constexpr (std::is_void_v<opt_t>) {
            if (p) f(*p);
        } else {
            opt_t r;
            if (p) r << f(*p);
            return r;
        }
    }

    template <typename F>
    decltype (auto) operator >>= (F f) &&
    {
        using opt_t = join_t<opt, F, T&&>;
        if constexpr (std::is_void_v<opt_t>) {
            if (p) f(std::move(*p));
        } else {
            opt_t r;
            if (p) r << f(std::move(*p));
            return r;
        }
    }

    iter begin() const { return iter{p ? &p : nullptr}; }
    unit end() const { return nil; }

private:
    std::optional<T> p;
};

template <typename T, typename... U>
opt<T> wrap_opt(U&&... t)
{
    return opt<T>{std::in_place, std::forward<U>(t)...};
}

template <typename T>
struct mnd<opt<T>> : public std::true_type
{
    typedef T flat_type;
};

template<class T>
opt(T) -> opt<T>;

}
