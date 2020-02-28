// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <monad.h>
#include <optional_monad.h>

namespace cat {

template <typename T>
struct monad<std::vector<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct vector_monad : public std::false_type
{
    typedef T flat_type;
};

template <typename T>
struct vector_monad<std::vector<T>> : public std::true_type
{
    typedef T flat_type;
};


template <class T>
inline constexpr bool is_vector_v = vector_monad<T>::value;

template <typename R>
using flatten_vector_t =
    std::conditional_t<std::is_void_v<R>,
        void,
        std::conditional_t<is_vector_v<R>,
            R,
            std::conditional_t<is_optional_v<R>,
                std::vector<flatten_t<R>>,
                std::vector<R>
            >
        >
    >;

template <typename F, typename T>
using join_vector_t = flatten_vector_t<std::invoke_result_t<F, T>>;


template <typename T>
std::vector<T>& operator << (std::vector<T>& lhs, T&& rhs)
{
    lhs.emplace_back(std::move(rhs));
    return lhs;
}

template <typename T>
std::vector<T>& operator << (std::vector<T>& lhs, std::optional<T>&& rhs)
{
    if (rhs) lhs.emplace_back(std::move(*rhs));
    return lhs;
}

template <typename T>
std::vector<T>& operator << (std::vector<T>& lhs, std::unique_ptr<T>&& rhs)
{
    if (rhs) lhs.emplace_back(std::move(*rhs));
    return lhs;
}

template <typename T>
std::vector<T>& operator << (std::vector<T>& lhs, std::shared_ptr<T>&& rhs)
{
    if (rhs) lhs.emplace_back(std::move(*rhs));
    return lhs;
}

template <typename T>
std::vector<T>& operator << (std::vector<T>& lhs, std::weak_ptr<T>&& rhs)
{
    if (auto safe = rhs.lock()) lhs.emplace_back(std::move(*safe));
    return lhs;
}

template <typename T>
std::vector<T>& operator << (std::vector<T>& lhs, std::future<T>&& rhs)
{
    if (rhs.valid()) {
        try {
           lhs.emplace_back(std::move(rhs.get()));
        } catch (...) {}
    }
    return lhs;
}

template <typename T>
std::vector<T>& operator << (std::vector<T>& lhs, std::shared_future<T>&& rhs)
{
    if (rhs.valid()) {
        try {
           lhs.emplace_back(std::move(rhs.get()));
        } catch (...) {}
    }
    return lhs;
}

template <typename T>
std::vector<T>& operator << (std::vector<T>& lhs, std::vector<T>&& rhs)
{
    lhs.insert(lhs.end(),
             std::move_iterator(rhs.begin()),
             std::move_iterator(rhs.end()));
    return lhs;
}


template <typename T, typename F, typename Vector = join_vector_t<F, T&>>
Vector operator >>= (const std::vector<T>& lhs, F f)
{
    if constexpr (std::is_void_v<Vector>) {
        for (const auto& i : lhs) f(i);
    } else {
        Vector v;
        for (const auto& i : lhs)
            v << f(i);
        return v;
    }
}

template <typename T, typename F, typename Vector = join_vector_t<F, T&>>
Vector operator >>= (std::vector<T>&& lhs, F f)
{
    if constexpr (std::is_void_v<Vector>) {
        for (auto& i : lhs) f(std::move(i));
    } else {
        Vector v;
        for (auto& i : lhs)
            v << f(std::move(i));
        return v;
    }
}

}
