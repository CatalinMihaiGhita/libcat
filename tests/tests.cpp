// Copyright (C) 2018 Ghita Catalin Mihai
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cat>

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

struct A
{
    owned<int> a{std::in_place, 0};
    /* data */
};


void asda(const int& a)
{

}

TEST(Cat, owned)
{
    owned<int> i{std::in_place, 0};
    owned<mockfoo> z{std::in_place, 0};
    z->val = 1;

    auto m = some<owned<int>>(1);
    for (int& i : m) {
        i = 10;
    }
    auto xx = std::move(m);
    asda(*i);

    maybe<shared<rwlock<int>>> mm{std::in_place, 1};
    for (auto&& mm_cell : mm) {
        for (int& j : mm_cell.try_lock_mut()) {
            j = 1;
        }
    }

    maybe<int> ms{std::in_place, 1};
    for (auto&& msi : ms) {
        ms = empty;
        msi = 1;
    }
}

/*
TEST(Cat, sh)
{
    sh<rw<int>> i{0};
    sh<mockfoo> z{0};
    *i = 1;

    {
        const mut ra{*i};
        *ra = 2;
    }

}
*/
/*

TEST(Cat, Nil)
{
    static_assert (std::is_empty_v<unit>);
    static_assert (std::is_default_constructible_v<unit>);
    static_assert (std::is_copy_constructible_v<unit>);
    static_assert (std::is_copy_assignable_v<unit>);
    static_assert (std::is_move_constructible_v<unit>);
    static_assert (std::is_move_assignable_v<unit>);

    unit n1;
    unit n2 = nil;
    n2 = n1;
    EXPECT_EQ(sizeof(unit), 1);
    EXPECT_TRUE(n1 == n2);
    EXPECT_FALSE(n1 != n2);
}

nvr throw_exception() { throw std::bad_exception{}; }

TEST(Cat, Nvr)
{
    static_assert (std::is_empty_v<nvr>);
    static_assert (!std::is_default_constructible_v<nvr>);
    static_assert (std::is_copy_constructible_v<nvr>);
    static_assert (std::is_copy_assignable_v<nvr>);
    static_assert (std::is_move_constructible_v<nvr>);
    static_assert (std::is_move_assignable_v<nvr>);

    EXPECT_THROW(throw_exception(), std::bad_exception);
}

void test_box(box<mockfoo> b, int t)
{
    EXPECT_EQ(b->val, t);
    EXPECT_EQ((*b).val, t);
}

TEST(Cat, Box)
{
    static_assert (!std::is_default_constructible_v<box<mockfoo>>);
    static_assert (!std::is_copy_constructible_v<box<mockfoo>>);
    static_assert (!std::is_copy_assignable_v<box<mockfoo>>);
    static_assert (std::is_move_constructible_v<box<mockfoo>>);
    static_assert (std::is_move_assignable_v<box<mockfoo>>);

    {
        box<mockfoo> s{wrap_box<mockfoo>(0)};
        EXPECT_EQ(s->val, 0);
        EXPECT_EQ((*s).val, 0);
        test_box(std::move(s), 0);
    }
    {
        box<mockfoo> s{std::in_place, 0};
        s = wrap_box<mockfoo>(1);
        EXPECT_EQ(s->val, 1);
        EXPECT_EQ((*s).val, 1);
        test_box(std::move(s), 1);
    }
}*/
/*
TEST(Cat, Rc)
{
    static_assert (!std::is_default_constructible_v<rc<mockfoo>>);
    static_assert (!std::is_copy_constructible_v<rc<mockfoo>>);
    static_assert (!std::is_copy_assignable_v<rc<mockfoo>>);
    static_assert (std::is_move_constructible_v<rc<mockfoo>>);
    static_assert (std::is_move_assignable_v<rc<mockfoo>>);

    rc<mockfoo> s{wrap_rc<mockfoo>(0)};

    EXPECT_EQ(s->val, 0);
    EXPECT_EQ((*s).val, 0);
    rc<mockfoo> o{++s};
    EXPECT_EQ(o->val, 0);
    EXPECT_EQ((*o).val, 0);
}*/

/*
TEST(Cat, Lnk)
{
    link<mockfoo> s{wrap_link<mockfoo>(0)};
    EXPECT_EQ(s->val, 0);
    EXPECT_EQ((*s).val, 0);
    link<mockfoo> o{s};
    EXPECT_EQ(o->val, 0);
    EXPECT_EQ((*o).val, 0);
}
*/
/*
TEST(Cat, Opt)
{
    static_assert (std::is_default_constructible_v<opt<mockfoo>>);
    static_assert (std::is_copy_constructible_v<opt<mockfoo>>);
    static_assert (std::is_copy_assignable_v<opt<mockfoo>>);
    static_assert (std::is_move_constructible_v<opt<mockfoo>>);
    static_assert (std::is_move_assignable_v<opt<mockfoo>>);

    auto s = wrap_opt<mockfoo>(0);
    auto new_opt = s >>= [] (const mockfoo& s) {
        EXPECT_EQ(s.val, 0);
        return 0;
    } >>= [] (int i) {
       EXPECT_EQ(i, 0);
       return i + 1;
    };
    new_opt >>= [] (int i) {
        EXPECT_EQ(i, 1);
    };
    s << unit{};
}

TEST(Cat, OptBox)
{
    static_assert (std::is_default_constructible_v<opt<box<mockfoo>>>);
    static_assert (!std::is_copy_constructible_v<opt<box<mockfoo>>>);
    static_assert (!std::is_copy_assignable_v<opt<box<mockfoo>>>);
    static_assert (std::is_move_constructible_v<opt<box<mockfoo>>>);
    static_assert (std::is_move_assignable_v<opt<box<mockfoo>>>);

    opt<box<mockfoo>> b{wrap_box<mockfoo>(0)};
    b >>= [] (const box<mockfoo>& s) {
        EXPECT_EQ(s->val, 0);
        return 0;
    };
    for (const box<mockfoo>& s : b) {
        EXPECT_EQ(s->val, 0);
    }
    b << nil;

}*/
/*
TEST(Cat, Wk)
{
    static_assert (std::is_default_constructible_v<opt<rc<mockfoo>>>);
    static_assert (!std::is_copy_constructible_v<opt<rc<mockfoo>>>);
    static_assert (!std::is_copy_assignable_v<opt<rc<mockfoo>>>);
    static_assert (std::is_move_constructible_v<opt<rc<mockfoo>>>);
    static_assert (std::is_move_assignable_v<opt<rc<mockfoo>>>);

    auto owner = wrap_rc<mockfoo>(0);
    opt<rc<mockfoo>> s{owner};
    auto new_opt = s >>= [] (const rc<mockfoo>& s) {
        EXPECT_EQ(s->val++, 0);
        return wrap_opt<int>(2);
    };
    for (const rc<mockfoo>& i : s) {
        EXPECT_EQ(i->val++, 1);
    }
    for (int i : new_opt) {
        EXPECT_EQ(i, 2);
    }

    opt<rc<mockfoo>> ss{++s};
    for (const rc<mockfoo>& i : ss) {
        EXPECT_EQ(i->val, 2);
    }

    EXPECT_EQ(owner->val, 2);
    ss << nil;
}

TEST(Cat, Lazy)
{
    static_assert ( std::is_default_constructible_v<lazy<mockfoo>>);
    static_assert (!std::is_copy_constructible_v   <lazy<mockfoo>>);
    static_assert (!std::is_copy_assignable_v      <lazy<mockfoo>>);
    static_assert ( std::is_move_constructible_v   <lazy<mockfoo>>);
    static_assert ( std::is_move_assignable_v      <lazy<mockfoo>>);

    auto l = wrap_lazy<int>(100);
    l >>= [] (int t) -> void {
           EXPECT_EQ(t, 100);
    };

    lazy<int> generator2;
    lazy<int> generator1;
    auto composed_lazy = generator1 >>= [g = ++generator2] (int t) {
        static unsigned tries = 1;
        switch (tries) {
        case 1:
            EXPECT_EQ(t, 55);
            break;
        case 2:
            EXPECT_EQ(t, 66);
            break;
        case 3:
        case 4:
            EXPECT_EQ(t, 77);
            break;
        default: {
            EXPECT_TRUE(false);
            break;
            }
        }
        ++tries;
        return ++g;
    };

    int executed = 0;
    composed_lazy >>= [&] (int f) {
        ++executed;
         EXPECT_EQ(f, executed);
    };

    generator1 << 55;
    generator1 << 66;

    auto lazy_identity= wrap_lazy<int>(77);
    generator1 << lazy_identity;
    generator1 << std::move(lazy_identity);
    // lazy_identity is in never state now
    generator1 << lazy_identity;

    EXPECT_EQ(executed, 0);

    generator2 << 1;
    generator2 << 2;
    generator2 << 3;
    generator2 << 4;

    EXPECT_EQ(executed, 4);

    generator2.abort();

    generator2 << 1;

    EXPECT_EQ(executed, 4);
}

TEST(Cat, Vec)
{
    vec<int> v{1, 2, 3, 4, 5};
    auto new_v = v >>= [] (int i) -> opt<int> {
        if ( i % 2 == 0) {
            return i;
        }
        return {};
    };

    ASSERT_EQ(new_v.size(), 2);
    for (auto&& i : new_v) {
        EXPECT_EQ(i % 2, 0);
    }

    auto rcv = wrap_rc<vec<int>>();
    *rcv << 2 << 3 << 4;
    lazy<int> gen;
    rcv << gen;
    ASSERT_EQ(rcv->size(), 3);
    gen << 1;
    ASSERT_EQ(rcv->size(), 4);
}
*/
