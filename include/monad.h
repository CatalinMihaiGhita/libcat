// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <functional>

namespace cat {

template <template<class> class M, class T>
struct flatten
{
     typedef T type;
};

template <template<class> class M, class T>
struct flatten<M, M<T>>
{
    typedef T type;
};
template <template<class> class M, class T>
using flatten_t = typename flatten<M, T>::type;

template <template<class> class M, typename F, typename T>
using join_t = M<flatten_t<M, std::invoke_result_t<F,T>>>;

template <class T>
class is_monad : public std::false_type
{
};

template <class T>
inline constexpr bool is_monad_v = is_monad<T>::value;

}
