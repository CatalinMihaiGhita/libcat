// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <memory>
#include <tup.h>
#include <opt.h>

namespace cat {

template <typename T>
class rc
{
    template <typename U>
    friend class opt;

    rc(std::shared_ptr<T> t) : p(std::move(t)) {}
    constexpr rc(unit) {}
    void reset() { p.reset(); }

public:
    template <typename... U>
    explicit rc(std::in_place_t, U&&... t) : p(std::make_shared<T>(std::forward<U>(t)...)) {}

    rc(const rc<T> &) = delete;
    rc& operator=(const rc<T> &) = delete;

    template <typename U = T>
    rc(rc<U> &&t) : p(std::move(t.p)) {}
    template <typename U = T>
    rc& operator=(rc<U> &&t) { p = std::move(t.p); }

    T* operator->() const noexcept { return p.get(); }
    std::add_lvalue_reference_t<T> operator*() const { return *p; }

    rc<T> operator++() const { return p; }

private:
    template <typename U>
    friend class rc;

    std::shared_ptr<T> p;
};

template <typename T, typename... U>
rc<T> wrap_rc(U&&... t)
{
    return rc<T>{std::in_place, std::forward<U>(t)...};
}

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
        bool operator!=(unit) const { return p.operator->() != nullptr; }
        bool operator==(unit) const { return p.operator->() == nullptr; }
        void operator++() { p.reset(); }
    private:
        iter(std::shared_ptr<T>&& p) : p(std::move(p)) {}
        rc<T> p;
        friend class opt;
    };

    opt(const opt &t) = delete;
    opt& operator=(const opt &t) = delete;

    constexpr opt() {}
    constexpr opt(unit) {}

    opt(opt &&t) = default;
    opt& operator=(opt &&t) = default;

    template <typename U>
    opt(const rc<U>& u) : p(u.p) {}
    template <typename U>
    opt(const opt<rc<U>>& u) : p(u.p) {}
    template <typename U>
    opt(opt<rc<U>>&& u) : p(std::move(u.p)) {}

    template <typename U>
    explicit opt(std::in_place_t, const rc<U>& u) : p(u.p) {}

    opt& operator << (unit) { p.reset(); return *this; }
    template <typename U>
    opt& operator << (const rc<U>& u) { p = u.p; return *this;}
    template <typename U>
    opt& operator << (const opt<rc<U>>& u) { p = u.p; return *this;}
    template <typename U>
    opt& operator << (opt<rc<U>>&& u) { p = std::move(u.p); return *this;}

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
    unit end() const { return {}; }

    opt operator++() const { return p; }

private:
    std::weak_ptr<T> p;
};


}
