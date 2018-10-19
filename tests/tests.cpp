// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <gtest/gtest.h>

#include <iostream>
#include <cat.h>

using namespace cat;

TEST(Cat, Nil)
{
    nil n1;
    nil n2 = {};
    n2 = n1;
    EXPECT_EQ(sizeof(nil), 1);
    EXPECT_TRUE(n1 == n2);
    EXPECT_FALSE(n1 != n2);
}

TEST(Cat, Box)
{
    struct mock {
        int i;
    };
    box<mock> s = wrap_box<mock>(0);
    EXPECT_EQ(s->i, 0);
    EXPECT_EQ((*s).i, 0);
}
