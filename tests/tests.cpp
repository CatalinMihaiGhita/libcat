// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <gtest/gtest.h>

#include <cat.h>

using namespace cat;

struct mock {
    mock(int i) : i{i} {}
    int i;
};

TEST(Cat, Nil)
{
    nil n1;
    nil n2 = {};
    n2 = n1;
    EXPECT_EQ(sizeof(nil), 1);
    EXPECT_TRUE(n1 == n2);
    EXPECT_FALSE(n1 != n2);
}

nvr test() { throw std::bad_exception{}; }

TEST(Cat, Nvr)
{
    EXPECT_THROW(test(), std::bad_exception);
}

TEST(Cat, Box)
{
    box<mock> s{wrap_box<mock>(0)};
    EXPECT_EQ(s->i, 0);
    EXPECT_EQ((*s).i, 0);
    box<mock> o{std::move(s)};
}

TEST(Cat, Rc)
{
    rc<mock> s{wrap_rc<mock>(0)};
    EXPECT_EQ(s->i, 0);
    EXPECT_EQ((*s).i, 0);
    rc<mock> o{++s};
}

TEST(Cat, Opt)
{
    opt<mock> s{wrap_opt<mock>(0)};
    auto new_opt = s >>= [] (const mock& s) {
        EXPECT_EQ(s.i, 0);
        return 0;
    } >>= [] (int i) {
       EXPECT_EQ(i, 0);
       return i + 1;
    };
    new_opt >>= [] (int i) {
        EXPECT_EQ(i, 1);
    };
}

TEST(Cat, OptBox)
{
    opt<box<mock>> s{wrap_box<mock>(0)};
    s >>= [] (const box<mock>& s) {
        EXPECT_EQ(s->i, 0);
    };
}

TEST(Cat, OptRc)
{
    opt<rc<mock>> s{wrap_rc<mock>(0)};
    s >>= [] (const rc<mock>& s) {
        EXPECT_EQ(s->i, 0);
    };
}

lzy<float> generate()
{
    return {std::in_place, 10.0f};
}

TEST(Cat, Lzy)
{
    auto l = wrap_lzy<int>(100);
    l >>= [] (int t) {
           EXPECT_EQ(t, 100);
    };

    lzy<int> l2;
    auto f = l2 >>= [] (int t){
        static int tries = 1;
        switch (tries) {
        case 1:
            EXPECT_EQ(t, 55);
            break;
        case 2:
            EXPECT_EQ(t, 66);
            break;
        case 3:
            EXPECT_EQ(t, 77);
            break;
        default: {
            EXPECT_TRUE(false);
            break;
            }
        }
        ++tries;
        return generate();
    };

    int executed = 0;
    f >>= [&] (float f) {
         EXPECT_EQ(f, 10.0f);
         ++executed;
    };

    l2 << 55;
    l2 << 66;

    auto l3 = wrap_lzy<int>(77);
    l2 << l3;
    l2 << l3;

    EXPECT_EQ(executed, 3);
}

TEST(Cat, Vec)
{
    vec<int> v{1, 2, 3, 4, 5};
    auto new_v = v >>= [] (int i) {
        return i * 2;
    };
    for (auto&& i : new_v) {
        EXPECT_EQ(i % 2, 0);
    }
}
