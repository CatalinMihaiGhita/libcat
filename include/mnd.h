// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <functional>

namespace cat {

template <class T>
struct mnd : public std::false_type
{
    using flat_type = T;
};

template <class T>
inline constexpr bool is_mnd_v = mnd<T>::value;

template <typename R>
using flatten_t = typename mnd <R>::flat_type;

template <template<class> class M, typename R>
using mnd_or_void_t = std::conditional_t<std::is_void_v<R>, void, M<flatten_t<R>>>;

template <template<class> class M, typename F, typename T>
using join_t = mnd_or_void_t<M, std::invoke_result_t<F, T>>;

}
