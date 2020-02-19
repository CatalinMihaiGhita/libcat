// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <var.h>
#include <tup.h>

#include <mnd.h>
#include <opt.h>
#include <lazy.h>

#include <vector>

namespace cat {

template <typename T>
class vec
{
public:
    using flat_type = T;

    class iter
    {
    public:
        const T& operator*() const { return *it; }
        bool operator!=(iter other) const { return it != other.it; }
        bool operator==(iter other) const { return it == other.it; }
        void operator++() { it++; }

    private:
        iter(typename std::vector<T>::iterator i) : it(i) {}
        typename std::vector<T>::iterator it;
        friend class vec;
    };

    constexpr vec() {}
    constexpr vec(std::initializer_list<T> init) : p(std::move(init)) {}

    template <typename F>
    decltype(auto) operator >>= (F f) const
    {
        using vec_t = join_t<vec, F, const T&>;
        if constexpr (std::is_void_v<vec_t>) {
            for (auto&& i : p) f(i);
        } else {
            vec_t v;
            for (auto&& i : p) {
                v << f(i);
            }
            return v;
        }
    }

    vec<T>& operator << (T&& t)
    {
        p.emplace_back(std::move(t));
        return *this;
    }

    vec<T>& operator << (const T& t)
    {
        p.emplace_back(t);
        return *this;
    }

    vec<T>& operator << (vec<T>&& v)
    {
        p.insert(p.end(),
                 std::move_iterator(v.p.begin()),
                 std::move_iterator(v.p.end()));
        return *this;
    }

    vec<T>& operator << (unit)
    {
        return *this;
    }


    vec<T>& operator << (const vec<T>& v)
    {
        p.insert(p.end(),
                 v.p.begin(),
                 v.p.end());
        return *this;
    }

    iter begin()
    {
        return p.begin();
    }

    iter end()
    {
        return p.end();
    }

    size_t size() const noexcept
    {
        return p.size();
    }

private:
    std::vector<T> p;
};

template <typename T>
vec<T>& operator << (vec<T>& lhs, opt<T>&& rhs)
{
    std::move(rhs) >>= [&] (T&& v) {
        lhs << std::move(v);
    };
    return lhs;
}

template <typename T>
const rc<vec<T>>& operator << (const rc<vec<T>>& lhs, const lazy<T>& rhs)
{
    rhs >>= [lhs = opt<rc<vec<T>>>{lhs}] (const T& v) {
        for (auto&& flhs : lhs) {
            *flhs << std::move(v);
        };
    };
    return lhs;
}

template <typename T>
const rc<vec<T>>& operator << (const rc<vec<T>>& lhs, lazy<T>&& rhs)
{
    std::move(rhs) >>= [lhs = opt<rc<vec<T>>>{lhs}] (T&& v) {
        for (auto&& flhs : lhs) {
            *flhs << std::move(v);
        };
    };
    return lhs;
}

template <typename T>
struct mnd<vec<T>> : public std::true_type
{
    typedef T flat_type;
};

}
