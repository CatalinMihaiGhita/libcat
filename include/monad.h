// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

namespace cat {

template <template<class> class M, typename T>
struct join_decay
{
     typedef T type;
};

template <template<class> class M, typename T>
struct join_decay<M, M<T>>
{
    typedef T type;
};

template <template<class> class M, typename F, typename T>
using join = M<typename join_decay<M, decltype(std::declval<F>()(std::declval<T>()))>::type>;

class monad
{
};

}
