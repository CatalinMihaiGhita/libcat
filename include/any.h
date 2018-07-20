// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <variant>

namespace cat {

template <typename... T>
class any
{
public:
    constexpr any() {}
    template <typename U>
    constexpr any(U&& u) : t(std::forward<U>(u)) {}
    template <std::size_t I, typename... U>
    constexpr any(std::in_place_index_t<I> i, U&&... u)
        : t(i, std::forward<U>(u)...) {}

    constexpr std::size_t index() { return t.index(); }

    template <typename F>
    decltype(auto) operator >> (F f) const  {
        return std::visit(f, t);
    }
private:
    std::variant<T...> t;
};

template <>
class any<>
{
public:
    explicit any() = delete;
    [[noreturn]] any(const any&) { std::abort(); }
    [[noreturn]] any& operator=(const any&) { std::abort(); }
    template <typename T>
    [[noreturn]] operator T() const { std::abort(); }
};

constexpr bool operator<(const any<>&, const any<>&) noexcept { return false; }
constexpr bool operator>(const any<>&, const any<>&) noexcept { return false; }
constexpr bool operator<=(const any<>&, const any<>&) noexcept { return false; }
constexpr bool operator>=(const any<>&, const any<>&) noexcept { return false; }
constexpr bool operator==(const any<>&, const any<>&) noexcept { return false; }
constexpr bool operator!=(const any<>&, const any<>&) noexcept { return false; }

using nvr = any<>;

}
