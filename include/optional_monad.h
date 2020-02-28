// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <optional>
#include <memory>
#include <future>
#include "expected.h"

#include <monad.h>

namespace cat {

template <typename T>
struct monad<std::optional<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct monad<std::unique_ptr<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct monad<std::shared_ptr<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct monad<std::weak_ptr<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct monad<std::future<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct monad<std::shared_future<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct optional_monad : public std::false_type
{
    typedef T flat_type;
};

template <typename T>
struct optional_monad<std::optional<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct optional_monad<std::unique_ptr<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct optional_monad<std::shared_ptr<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct optional_monad<std::weak_ptr<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct optional_monad<std::future<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct optional_monad<std::shared_future<T>> : public std::true_type
{
    typedef T flat_type;
};

template <typename T>
struct optional_monad<expected<T>> : public std::true_type
{
    typedef T flat_type;
};

template <class T>
inline constexpr bool is_optional_v = optional_monad<T>::value;

template <typename R>
using flatten_optional_t =
    std::conditional_t<std::is_void_v<R>,
        void,
        std::conditional_t<is_optional_v<R>,
            R,
            std::optional<R>
        >
    >;


template <typename F, typename T>
using join_optional_t = flatten_optional_t<std::invoke_result_t<F, T>>;


template <typename T>
std::optional<T>& operator << (std::optional<T>& lhs, std::optional<T>&& rhs)
{
    if (rhs) lhs = std::move(rhs);
    return lhs;
}

template <typename T>
std::optional<T>& operator << (std::optional<T>& lhs, const std::optional<T>& rhs)
{
    if (rhs) lhs = rhs;
    return lhs;
}

template <typename T>
std::optional<T>& operator << (std::optional<T>& lhs, T&& rhs)
{
    lhs = std::move(rhs);
    return lhs;
}

template <typename T>
std::optional<T>& operator << (std::optional<T>& lhs, const T& rhs)
{
    lhs = rhs;
    return lhs;
}

template <typename T>
std::optional<T>& operator << (std::optional<T>& lhs, std::unique_ptr<T>&& rhs)
{
    if (rhs) lhs = std::move(*rhs);
    return lhs;
}

template <typename T>
std::optional<T>& operator << (std::optional<T>& lhs, std::shared_ptr<T>&& rhs)
{
    if (rhs) lhs = std::move(*rhs);
    return lhs;
}

template <typename T>
std::optional<T>& operator << (std::optional<T>& lhs, std::weak_ptr<T>&& rhs)
{
    if (auto sh = rhs.lock()) lhs = std::move(*sh);
    return lhs;
}

template <typename T>
std::optional<T>& operator << (std::optional<T>& lhs, std::future<T>& rhs)
{
    if (rhs.valid()) try { lhs = rhs.get(); } catch (...) {}
    return lhs;
}

template <typename T>
std::optional<T>& operator << (std::optional<T>& lhs, std::future<T>&& rhs)
{
    if (rhs.valid()) try { lhs = std::move(rhs.get()); } catch (...) {}
    return lhs;
}

template <typename T>
std::optional<T>& operator << (std::optional<T>& lhs, std::shared_future<T>&& rhs)
{
    if (rhs.valid()) try { lhs = std::move(rhs.get()); } catch (...) {}
    return lhs;
}

template <typename T, typename F, typename Optional = join_optional_t<F, T&>>
Optional operator >>= (const std::unique_ptr<T>& p, F&& f)
{
    if (p) return std::invoke(std::forward<F>(f), *p);
    return Optional();
}

template <typename T, typename F, typename Optional = join_optional_t<F, T&&>>
Optional operator >>= (std::unique_ptr<T>&& p, F&& f)
{
    if (p) return std::invoke(std::forward<F>(f), std::move(*p));
    return Optional();
}

template <typename T, typename F, typename Optional = join_optional_t<F, T&>>
Optional operator >>= (const std::shared_ptr<T>& p, F&& f)
{
    if (p) return std::invoke(std::forward<F>(f), *p);
    return Optional();
}

template <typename T, typename F, typename Optional = join_optional_t<F, T&&>>
Optional operator >>= (std::shared_ptr<T>&& p, F&& f)
{
    if (p) return std::invoke(std::forward<F>(f), std::move(*p));
    return Optional();
}

template <typename T, typename F, typename Optional = join_optional_t<F, T&>>
Optional operator >>= (const std::weak_ptr<T>& p, F&& f)
{
    if (auto safe = p.lock()) return std::invoke(std::forward<F>(f), *safe);
    return Optional();
}

template <typename T, typename F, typename Optional = join_optional_t<F, T&&>>
Optional operator >>= (std::weak_ptr<T>&& p, F&& f)
{
    if (auto safe = p.lock()) return std::invoke(std::forward<F>(f), std::move(*safe));
    return Optional();
}

template <typename T, typename F, typename Optional = join_optional_t<F, const T&>>
Optional operator >>= (const std::optional<T>& p, F&& f)
{
    if (p) return std::invoke(std::forward<F>(f), *p);
    return Optional();
}

template <typename T, typename F, typename Optional = join_optional_t<F, T&>>
Optional operator >>= (std::optional<T>& p, F f)
{
    if (p) return std::invoke(std::forward<F>(f), *p);
    return Optional();
}

template <typename T, typename F, typename Optional = join_optional_t<F, T&&>>
Optional operator >>= (std::optional<T>&& p, F f)
{
    if (p) return std::invoke(std::forward<F>(f), std::move(*p));
    return Optional();
}

}
