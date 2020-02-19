// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <tup.h>
#include <memory>
#include <opt.h>

namespace cat {

template <typename T>
class box
{
    template <typename U>
    friend class opt;

    constexpr box(unit) {}
public:

    template <typename... U>
    explicit box(std::in_place_t, U&&... t) : p(new T{std::forward<U>(t)...}) {}

    template <typename U = T>
    box(box<U>&& u) : p(std::move(u.p)) {}
    template <typename U = T>
    box& operator=(box<U> &&t) { p = std::move(t.p); }

    T* operator->() const noexcept { return p.get(); }
    std::add_lvalue_reference_t<T> operator*() const { return *p; }

private:
    template <class U>
    friend class box;
    std::unique_ptr<T> p;
};

template <typename T, typename... U>
box<T> wrap_box(U&&... t)
{
    return box<T>{std::in_place, std::forward<U>(t)...};
}

template <typename T>
class opt<box<T>>
{
public:
    class iter
    {
    public:
        const box<T>& operator*() const { return *p; }
        bool operator!=(unit) const { return p != nullptr; }
        bool operator==(unit) const { return p == nullptr; }
        void operator++() { p = nullptr; }

    private:
        iter(const box<T>* p) : p(p) {}
        const box<T>* p;
        friend class opt;
    };

    template <typename U>
    explicit opt(std::in_place_t, box<U>&& u) : p(std::move(u)) {}

    constexpr opt() : p(nil) {}
    constexpr opt(unit) : p(nil) {}

    opt(opt &&t) = default;
    opt& operator=(opt &&t) = default;

    opt(const opt &t) = delete;
    opt& operator=(const opt &t) = delete;

    template <typename U>
    opt(box<U>&& u) : p(std::move(u)) {}
    template <typename U>
    opt(opt<box<U>>&& u) : p(std::move(u.p)) {}

    template <typename U>
    opt& operator << (box<U>&& u) { p = std::move(u); return *this; }
    template <typename U>
    opt& operator << (opt<box<U>>&& u) { p = std::move(u.p); return *this; }
    opt& operator << (unit) { p = box<T>{nil}; return *this; }

    constexpr std::size_t index() const { return p.operator->() ? 0 : 1; }

    template <typename F>
    decltype (auto) operator >>= (F f) const
    {
        using opt_t = join_t<opt, F, const box<T>&>;
        if constexpr (std::is_void_v<join_t<opt, F, const box<T>&>>) {
            if (p.operator->()) {
                f(p);
            }
        } else {
            opt_t r;
            if (p.operator->()) r << f(p);
            return r;
        }
    }

    iter begin() const { return iter{p.operator->() ? &p : nullptr}; }
    unit end() const { return nil; }

private:
    box<T> p;
};

}
