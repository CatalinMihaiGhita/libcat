// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "expected.h"
#include <future>

#include <monad.h>

namespace cat {

template <typename T>
struct monad<expected<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct expected_monad : public std::false_type
{
    typedef T flat_type;
};

template <typename T>
struct expected_monad<expected<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct expected_monad<std::future<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct expected_monad<std::shared_future<T>> : public std::true_type
{
    typedef T flat_type;
};

template <class T>
inline constexpr bool is_expected_v = expected_monad<T>::value;

template <typename R>
using flatten_expected_t =
    std::conditional_t<std::is_void_v<R>,
        void,
        std::conditional_t<is_expected_v<R>,
            R,
            expected<R>
        >
    >;


template <typename F, typename T>
using join_expected_t = flatten_expected_t<std::invoke_result_t<F, T>>;


template <typename T>
expected<T>& operator << (expected<T>& lhs, expected<T>&& rhs)
{
    if (rhs.index()) lhs = std::move(rhs);
    return lhs;
}

template <typename T>
expected<T>& operator << (expected<T>& lhs, const expected<T>& rhs)
{
    if (rhs.index()) lhs = rhs;
    return lhs;
}

template <typename T>
expected<T>& operator << (expected<T>& lhs, T&& rhs)
{
    lhs = std::move(rhs);
    return lhs;
}

template <typename T>
expected<T>& operator << (expected<T>& lhs, const T& rhs)
{
    lhs = rhs;
    return lhs;
}

template <typename T>
expected<T>& operator << (expected<T>& lhs, std::future<T>& rhs)
{
    if (rhs.valid()) try { lhs = rhs.get(); } catch (...) { lhs = std::current_exception(); }
    return lhs;
}

template <typename T>
expected<T>& operator << (expected<T>& lhs, std::future<T>&& rhs)
{
    if (rhs.valid()) try { lhs = std::move(rhs.get()); } catch (...) { lhs = std::current_exception(); }
    return lhs;
}

template <typename T>
expected<T>& operator << (expected<T>& lhs, std::shared_future<T>&& rhs)
{
    if (rhs.valid()) try { lhs = std::move(rhs.get()); } catch (...) { lhs = std::current_exception(); }
    return lhs;
}

template <typename T, typename F, typename Expected = join_expected_t<F, T&>>
Expected operator >>= (const expected<T>& p, F&& f)
{
    if (p.index()) return std::invoke(std::forward<F>(f), std::get<T>(p));
    return std::get<std::exception_ptr>(p);
}

template <typename T, typename F, typename Expected = join_expected_t<F, T&&>>
Expected operator >>= (expected<T>&& p, F&& f)
{
    if (p.index()) return std::invoke(std::forward<F>(f), std::move(std::get<T>(p)));
    return std::get<std::exception_ptr>(p);
}

template <typename T, typename F, typename Expected = join_expected_t<F, T&>>
Expected operator >>= (std::future<T>& p, F&& f)
{
    if (p.valid()) try { return std::invoke(std::forward<F>(f), p.get());} catch (...) { return std::current_exception(); }
    return std::make_exception_ptr(std::runtime_error("Trying to bind invalid future"));
}

template <typename T, typename F, typename Expected = join_expected_t<F, T&&>>
Expected operator >>= (std::future<T>&& p, F&& f)
{
    if (p.valid()) try { return std::invoke(std::forward<F>(f), std::move(p.get()));} catch (...) { return std::current_exception(); }
    return std::make_exception_ptr(std::runtime_error("Trying to bind invalid future"));
}

template <typename T, typename F, typename Expected = join_expected_t<F, T&>>
Expected operator >>= (const std::shared_future<T>& p, F&& f)
{
    if (p.valid()) try { return std::invoke(std::forward<F>(f), p.get());} catch (...) { return std::current_exception();}
    return std::make_exception_ptr(std::runtime_error("Trying to bind invalid future"));
}

template <typename T, typename F, typename Expected = join_expected_t<F, T&&>>
Expected operator >>= (std::shared_future<T>&& p, F&& f)
{
    if (p.valid()) try { return std::invoke(std::forward<F>(f), std::move(p.get()));} catch (...) { return std::current_exception(); }
    return std::make_exception_ptr(std::runtime_error("Trying to bind invalid future"));
}

}
