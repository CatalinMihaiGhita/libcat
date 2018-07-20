// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <tuple>

namespace cat {

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

template <typename... T>
class all
{
public:
    constexpr all() {}
    template <typename... U>
    constexpr all(U&&... u) : t(std::forward<U>(u)...) {}
private:
    std::tuple<T...> t;
};

template<class... T>
all(T...) -> all<T...>;

template <>
class all<>
{
public:
    constexpr all() {}
    template <typename T> 
    constexpr all(T&&) {}
};

using nil = all<>;

}
