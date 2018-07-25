// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>
#include <cat.h>

using namespace cat;

opt<std::string> free_fn(const rc<std::string>& s)
{
    std::cout << *s << std::endl;
    return "second";
}

class A
{

};

class B : public A
{
public:
    B(box<A>&&){}
};

int main()
{
    box x = wrap_box<B>(wrap_box<A>());
    box<A> y = std::move(x);

    vec<int> v{1, 2};
    v >>= [] (int i) {
        vec<float> g;
        g << 1.0f;
        return g;
    };

    opt<rc<std::string>> t = wrap_rc<std::string>("first");
    t >>= free_fn >>= [] (const std::string &s) {
        std::cout << s << std::endl;
        return nil{};
    };

    lazy<int> i;
    i >>= [] (int i) {
        std::cout << i << std::endl;
        return nil{};
    } >>= [] (nil) {
        std::cout << "nil continuation!!" << std::endl;
        return nil{};
    };
    i << 9;

    return 0;
}
