// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <functional>

namespace cat {

template <class T>
struct monad : public std::false_type
{
    using flat_type = T;
};

template <class T>
inline constexpr bool is_monad_v = monad<T>::value;

template <typename R>
using flatten_t = typename monad <R>::flat_type;

}
