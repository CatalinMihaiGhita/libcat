// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <memory>

#include <rc.h>

namespace cat {

template <typename T>
using wk = opt<rc<T>>;

template <typename T>
class opt<rc<T>>
{
    opt(const std::weak_ptr<T> &t) : p(t) {}

public:
    using flat_type = rc<T>;

    class iter
    {
    public:
        const rc<T>& operator*() const { return p; }
        bool operator!=(nil) const { return p.operator->() != nullptr; }
        bool operator==(nil) const { return p.operator->() == nullptr; }
        void operator++() { p.reset(); }
    private:
        iter(std::shared_ptr<T>&& p) : p(std::move(p)) {}
        rc<T> p;
        friend class opt;
    };

    opt(const opt &t) = delete;
    opt& operator=(const opt &t) = delete;

    constexpr opt() {}
    constexpr opt(nil) {}

    opt(opt &&t) = default;
    opt& operator=(opt &&t) = default;

    template <typename U>
    opt(const rc<U>& u) : p(u.p) {}
    template <typename U>
    opt(const wk<U>& u) : p(u.p) {}
    template <typename U>
    opt(wk<U>&& u) : p(std::move(u.p)) {}

    template <typename U>
    explicit opt(std::in_place_t, const rc<U>& u) : p(u.p) {}

    opt& operator << (nil) { p.reset(); return *this; }
    template <typename U>
    opt& operator << (const rc<U>& u) { p = u.p; return *this;}
    template <typename U>
    opt& operator << (const wk<U>& u) { p = u.p; return *this;}
    template <typename U>
    opt& operator << (wk<U>&& u) { p = std::move(u.p); return *this;}

    template <typename U>
    opt(rc<U>&&) = delete;
    template <typename U>
    opt(std::in_place_t, rc<U>&&) = delete;
    template <typename U>
    opt& operator << (rc<U>&& u) = delete;

    constexpr std::size_t index() const { return !p.expired(); }

    template <typename F>
    decltype (auto) operator >>= (F f) const
    {
        using opt_t = join_t<opt, F, const rc<T>&>;
        if constexpr (std::is_void_v<join_t<opt, F, const rc<T>&>>) {
            if (auto g = p.lock()) f(g);
        } else {
            opt_t r;
            if (auto g = p.lock()) r << f(g);
            return r;
        }
    }

    iter begin() const {return iter{p.lock()}; }
    nil end() const { return {}; }

    opt operator++() const { return p; }

private:
    std::weak_ptr<T> p;
};

}
