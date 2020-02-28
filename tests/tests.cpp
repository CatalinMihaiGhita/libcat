// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cat.h>

using namespace cat;

struct foo
{
    virtual ~foo();
};

foo::~foo()
{}

struct mockfoo : public foo
{
    mockfoo(int i);
    ~mockfoo() override;
    mockfoo(const mockfoo&);
    mockfoo& operator=(const mockfoo&);

    MOCK_METHOD0(Die, void());
    int val;
    int val2;
};

mockfoo::mockfoo(int i)
    : val(i)
{
    EXPECT_CALL(*this, Die());
}

mockfoo::~mockfoo()
{
    Die();
}

mockfoo::mockfoo(const mockfoo& o)
    : val(o.val)
{}

mockfoo& mockfoo::operator=(const mockfoo& o)
{
    val = o.val;
    return *this;
}

struct Boo
{
    int f();
};

namespace cat {

template <typename T>
using shared_ptr = std::shared_ptr<const T>;

}

TEST(Cat, Nil)
{
    std::optional<int> i;
    std::future<int> j;
    i << j;

    j >>= [] (int k) {
        return k;
    };
}

TEST(Cat, Vec)
{
    std::vector<int> h{1, 2, 3, 4, 2, 3, 4, 4, 4, 3};
    auto f = h >>= [] (int i) {
        return std::async(std::launch::async, [] { throw 1; return 0;});
    };
    EXPECT_EQ(f.size(), 4);
}
