// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <optional>

#include <any.h>
#include <all.h>
#include <box.h>
#include <rc.h>
#include <mnd.h>

namespace cat {

template <typename T>
using opt = any<T, nil>;

template <typename T>
class is_monad<opt<T>> : public std::true_type
{
};

template <typename T>
class any<box<T>, nil>
{
public:
    class iter
    {
    public:
        const box<T>& operator*() const { return *p; }
        bool operator!=(nil) const { return p != nullptr; }
        bool operator==(nil) const { return p == nullptr; }
        void operator++() { p = nullptr; }

    private:
        iter(const box<T>* p) : p(p) {}
        const box<T>* p;
        friend class any;
    };

    constexpr any() : p(nil{}) {}
    constexpr any(nil n) : p(n) {}
    any& operator << (nil n) { p = box<T>{n}; }

    template <typename U>
    any(box<U>&& u) : p(std::move(u)) {}
    template <typename U>
    any& operator << (box<U>&& u) { p = std::move(u); }

    constexpr std::size_t index() const { return p.operator->() ? 0 : 1; }

    template <typename F>
    join_t<opt, F, const box<T>&> operator >>= (F f) const
    {
        if (p.operator->())
            return f(p);
        else
            return {};
    }

    iter begin() const { return iter{p.operator->() ? &p : nullptr}; }
    nil end() const { return {}; }

private:
    box<T> p;
};

template <typename T>
class any<rc<T>, nil>
{
public:
    class iter
    {
    public:
        const rc<T>& operator*() const { return *p; }
        bool operator!=(nil) const { return p != nullptr; }
        bool operator==(nil) const { return p == nullptr; }
        void operator++() { p = nullptr; }
    private:
        iter(const rc<T>* p) : p(p) {}
        const rc<T>* p;
        friend class any;
    };

    constexpr any() : p(nil{}) {}
    constexpr any(nil n) : p(n) {}
    any& operator << (nil n) { p = rc<T>{n}; }
    
    any(const any&) = delete;
    any& operator=(const any&) = delete;

    template <typename U>
    any(rc<U>&& u) : p(std::move(u)) {}
    template <typename U>
    any& operator << (rc<U>&& u) { p = std::move(u); }

    constexpr std::size_t index() const { return p.operator->() ? 0 : 1; }

    template <typename F>
    join_t<opt, F, const rc<T>&> operator >>= (F f) const
    {
        if (p.operator->())
            return f(p);
        else
            return {};
    }

    iter begin() const {return iter{p.operator->() ? &p : nullptr}; }
    nil end() const { return {}; }

    any operator++() const { return ++p; }

private:
    rc<T> p;
};

template <typename T>
class any<T, nil>
{
public:
    class iter
    {
    public:
        const T& operator*() const { return **p; }
        bool operator!=(nil) const { return p != nullptr; }
        bool operator==(nil) const { return p == nullptr; }
        void operator++() { p = nullptr; }
    private:
        iter(const std::optional<T>* p) : p(p) {}
        const std::optional<T>* p;
        friend class any;
    };

    constexpr any() {}
    constexpr any(nil) {}
    any& operator << (nil) { p.reset(); }

    template <typename... U>
    any(std::in_place_t, U&&... u) : p(std::forward<U>(u)...) {}
    template <typename U>
    any& operator << (U&& u) { p = std::forward(u); }

    constexpr std::size_t index() const { return p.operator->() ? 0 : 1; }

    template <typename F>
    join_t<opt, F, const T&> operator >>= (F f) const
    {
        if (p)
            return f(*p);
        else
            return {};
    }

    iter begin() const { return iter{p ? &p : nullptr}; }
    nil end() const { return {}; }

private:
    std::optional<T> p;
};

template <typename T, typename... U>
opt<T> wrap_opt(U&&... t)
{
    return {std::in_place, std::forward<U>(t)...};
}

}
