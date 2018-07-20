// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>
#include <cat.h>

using namespace cat;

opt<std::string> call(const box<std::string>& s)
{
    return "";
}

int main()
{
    vec<int> v;
    v += 1;
    v >>= [] (int i) {
        vec<float> g;
        g += 1.0f;
        return g;
    };

    opt<box<std::string>> t = "A";
    for (auto&& s : t) {
        for (auto&& x : call(s)) {
            std::cout << x;
        }
    }

    return 0;
}
