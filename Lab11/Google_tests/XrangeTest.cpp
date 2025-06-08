#include "..\Xrange.h"
#include "gtest/gtest.h"
#include <vector>

TEST (XrangeTest, two_param) {
    auto x = xrange(4, 7);
    std::vector<int> v{x.begin(), x.end()};
    std::vector<int> ans = {4, 5, 6};
    ASSERT_EQ(v, ans);
}

TEST (XrangeTest, three_param) {
    auto x = xrange(2, 7, 2);
    std::vector<int> v{x.begin(), x.end()};
    std::vector<int> ans = {2, 4, 6};
    ASSERT_EQ(v, ans);
}

TEST (XrangeTest, three_param_minus) {
    auto x = xrange(-3, 5, 2);
    std::vector<int> v{x.begin(), x.end()};
    std::vector<int> ans = {-3, -1, 1, 3};
    ASSERT_EQ(v, ans);
}

TEST (XrangeTest, one_param) {
    auto x = xrange(6);
    std::vector<int> v{x.begin(), x.end()};
    std::vector<int> ans = {0, 1, 2,  3, 4, 5};
    ASSERT_EQ(v, ans);
}

TEST(XrangeTest, negative_step_1) {
    auto x = xrange(12, 8, -1);
    std::vector<int> v{x.begin(), x.end()};
    std::vector<int> ans = {12, 11, 10, 9};
    ASSERT_EQ(v, ans);
}

TEST(XrangeTest, negative_step_2) {
    auto x = xrange(12, 7, -2);
    std::vector<int> v{x.begin(), x.end()};
    std::vector<int> ans = {12, 10, 8};
    ASSERT_EQ(v, ans);
}

