// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <functional>

#include <monad.h>

namespace cat {
namespace kernel {

template <typename T, bool M>
struct fn;

template <typename R, typename... Args>
struct fn<R(*)(Args...), false>
{
    typedef R result_type;
};

template <typename T>
struct fn_object;

template <typename T>
struct fn<T, false> : public fn_object<decltype(&T::operator())>
{
};

template <typename O, typename R, typename... Args>
struct fn_object<R(O::*)(Args...) const>
{
    typedef R result_type;
};

template <typename O, typename R, typename... Args>
struct fn_object<R(O::*)(Args...)>
{
    typedef R result_type;
};

template <typename T, typename U, typename R>
struct fmap
{
    template <class... Args>
    decltype(auto) operator()(Args&&... args)
    {
        return u(t(std::forward<Args>(args)...));
    }
    T t; U u;
};

template <typename T, typename U>
struct fmap<T, U, void>
{
    template <class... Args>
    decltype(auto) operator()(Args&&... args)
    {
        t(std::forward<Args>(args)...);
        return u();
    }
    T t; U u;
};

template <typename T, typename U>
struct bind
{
    template <class... Args>
    decltype(auto) operator()(Args&&... args)
    {
        return t(std::forward<Args>(args)...) >>= u;
    }
    T t; U u;
};

template <typename F
         ,typename G
         ,typename R = typename fn<std::decay_t<F>, is_monad_v<std::decay_t<F>>>::result_type>
using map_t = typename std::conditional_t<is_monad_v<R>,
                                         bind<std::decay_t<F>, std::decay_t<G>>,
                                         fmap<std::decay_t<F>, std::decay_t<G>, R>>;

}

template <typename F
         ,typename G
         ,typename M = kernel::map_t<F,G>>
M operator >>= (F&& f, G&& g)
{
    return M{ std::forward<F>(f), std::forward<G>(g) };
}

}
