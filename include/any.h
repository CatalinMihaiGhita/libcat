// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <cstdlib>
#include <variant>

namespace cat {

template <typename... T>
using any = std::variant<T...>;

class nvr
{
public:
    explicit nvr() = delete;
    [[noreturn]] nvr(const nvr&) { std::abort(); }
    [[noreturn]] nvr& operator=(const nvr&) { std::abort(); }
    template <typename T>
    [[noreturn]] operator T() const { std::abort(); }
};

constexpr bool operator<(const nvr&, const nvr&) noexcept { return false; }
constexpr bool operator>(const nvr&, const nvr&) noexcept { return false; }
constexpr bool operator<=(const nvr&, const nvr&) noexcept { return false; }
constexpr bool operator>=(const nvr&, const nvr&) noexcept { return false; }
constexpr bool operator==(const nvr&, const nvr&) noexcept { return false; }
constexpr bool operator!=(const nvr&, const nvr&) noexcept { return false; }

}
