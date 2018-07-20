// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>
#include <cat.h>

using namespace cat;

opt<std::string> free_fn(const box<std::string>& s)
{
    std::cout << *s << std::endl;
    return "second";
}

int main()
{
    vec<int> v;
    v << 1;
    v >>= [] (int i) {
        vec<float> g;
        g << 1.0f;
        return g;
    };

    opt<box<std::string>> t = "first";
    t >>= free_fn >>= [] (const std::string &s) {
        std::cout << s << std::endl;
        return nil{};
    };

    return 0;
}
