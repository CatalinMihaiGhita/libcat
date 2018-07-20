// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <functional>

#include <monad.h>
#include <all.h>
#include <any.h>
#include <box.h>
#include <rc.h>
#include <opt.h>
#include <lazy.h>
#include <vec.h>

namespace cat {
namespace kernel {

template <typename T>
struct lambda_decay;

template <typename T, typename O = decltype(&T::operator())>
struct lambda : public lambda_decay<O>
{
};

template <typename ClassType, typename ReturnType, typename... Args>
struct lambda_decay<ReturnType(ClassType::*)(Args...) const>
{
    typedef ReturnType result_type;
};

template <typename ClassType, typename ReturnType, typename... Args>
struct lambda_decay<ReturnType(ClassType::*)(Args...)>
{
    typedef ReturnType result_type;
};

template <typename ReturnType, typename... Args>
struct lambda_decay<ReturnType(*)(Args...)>
{
    typedef ReturnType result_type;
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

template <typename T
         ,typename U
         ,typename DT = typename std::decay_t<T>
         ,typename DU = typename std::decay_t<U>
         ,typename R = typename lambda<DT>::result_type>
using map = typename std::conditional_t<std::is_base_of_v<monad, R>, bind<DT, DU>, fmap<DT, DU, R>>;

}

template <typename F
         ,typename G
         ,typename M = kernel::map<F,G>>
M operator>>=(F&& f, G&& g)
{
    return M{ std::forward<F>(f), std::forward<G>(g) };
}

}
