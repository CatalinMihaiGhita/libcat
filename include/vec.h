// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <any.h>
#include <all.h>

#include <mnd.h>

#include <vector>

namespace cat {

template <typename T>
class vec
{
public:
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
        if constexpr (std::is_void_v<join_t<vec, F, const T&>>) {
            for (auto&& i : p) {
                f(i);
            }
        } else {
            join_t<vec, F, const T&> v;
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

private:
    std::vector<T> p;
};

template <typename T>
class is_monad<vec<T>> : public std::true_type
{
};

}
