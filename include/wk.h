// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <memory>

#include <mnd.h>

#include <rc.h>
#include <opt.h>
#include <any.h>

namespace cat {

template <class T>
using wk = opt<rc<T>>;

template <typename T>
class any<rc<T>, nil>
{
public:
    class iter
    {
    public:
        const rc<T>& operator*() const { return p; }
        bool operator!=(nil) const { return p != nullptr; }
        bool operator==(nil) const { return p == nullptr; }
        void operator++() { p.reset(); }
    private:
        iter(std::shared_ptr<T>&& p) : p(std::move(p)) {}
        std::shared_ptr<T> p;
        friend class any;
    };

    any(const any &t) = delete;
    any& operator=(const any &t) = delete;

    constexpr any() {}
    constexpr any(nil) {}
    any& operator << (nil) { p.reset(); }

    template <typename U>
    any(const rc<U>& u) : p(u.p) {}
    template <typename U>
    any& operator << (const rc<U>& u) { p = u.p; }

    constexpr std::size_t index() const { return !p.expired(); }

    template <typename F>
    join_t<opt, F, const rc<T>&> operator >>= (F f) const
    {
        if constexpr (std::is_void_v<join_t<opt, F, const rc<T>&>>) {
            if (auto g = p.lock()) {
                f(g);
            }
        } else {
            if (auto g = p.lock())
                return f(g);
            else
                return {};
        }
    }

    iter begin() const {return iter{p.lock()}; }
    nil end() const { return {}; }

    any operator++() const { return ++p; }

private:
    std::weak_ptr<T> p;
};

}
