#include "gtest/gtest.h"
#include "../CircBufExt.h"

TEST(TestSuiteBufExt, PushBackTest) {
    CCircularBufferExt<int, std::allocator<int>> x (5, 0);

    x.push_back(1);
    ASSERT_EQ(x.max_size(), 10);
    x.push_back(2);
    ASSERT_EQ(x.max_size(), 10);
    x.push_back(3);
    x.push_back(4);

    ASSERT_EQ(x[0], 0);
    ASSERT_EQ(x[4], 0);
    ASSERT_EQ(x[5], 1);
    ASSERT_EQ(x[8], 4);

    ASSERT_EQ(x.size(), 9);
}

TEST(TestSuiteBufExt, PushFrontTest) {
    CCircularBufferExt<int, std::allocator<int>> x (5, 0);

    x.push_front(1);
    ASSERT_EQ(x.max_size(), 10);
    x.push_front(2);
    ASSERT_EQ(x.max_size(), 10);
    x.push_front(3);
    x.push_front(4);

    ASSERT_EQ(x[0], 4);
    ASSERT_EQ(x[1], 3);
    ASSERT_EQ(x[3], 1);
    ASSERT_EQ(x[4], 0);
    ASSERT_EQ(x[5], 0);
    ASSERT_EQ(x[8], 0);

    ASSERT_EQ(x.size(), 9);
}


TEST(TestSuiteBufExt, Insert1Test) {
    CCircularBufferExt<int, std::allocator<int>> x (5, 0);
    for (int i = 1; i < 5; ++i) {
        x.pop_front();
    }
    for (int i = 1; i < 5; ++i) {
        x.push_back(i);
    }
    x.insert(x.cbegin() + 1, 2, 9);

    ASSERT_EQ(x.size(), 7);
    ASSERT_EQ(x.max_size(), 10);

    ASSERT_EQ(x[0], 0);
    ASSERT_EQ(x[1], 9);
    ASSERT_EQ(x[2], 9);
    ASSERT_EQ(x[3], 1);
    ASSERT_EQ(x[6], 4);
}

TEST(TestSuiteBufExt, Insert2Test) {
    CCircularBufferExt<int, std::allocator<int>> x (5, 0);
    for (int i = 1; i < 6; ++i) {
        x.pop_front();
    }
    for (int i = 1; i < 6; ++i) {
        x.push_back(i);
    }


    CCircularBufferExt<int, std::allocator<int>> y (10, 0);
    y.pop_front();
    y.pop_front();
    y.push_back(1);

    y.insert(y.cend() - 3, x.begin() + 2, 1 + x.end());

    ASSERT_EQ(y.size(), 13);
    ASSERT_EQ(y.max_size(), 20);

    ASSERT_EQ(y[0], 0);
    ASSERT_EQ(y[5], 0);
    ASSERT_EQ(y[6], 3);
    ASSERT_EQ(y[8], 5);
    ASSERT_EQ(y[9], 1);
    ASSERT_EQ(y[10], 0);
    ASSERT_EQ(y[12], 1);
}

TEST(TestSuiteBufExt, AlgoTest) {
    CCircularBufferExt<int, std::allocator<int>> x (5, 0);
    for (int i = 1; i < 6; ++i) {
        x.pop_front();
    }
    x.push_back(1);
    x.push_back(5);
    x.push_back(2);
    x.push_back(3);
    x.push_back(5);

    std::sort(x.begin(), x.end());

    ASSERT_EQ(x[0], 1);
    ASSERT_EQ(x[1], 2);
    ASSERT_EQ(x[2], 3);
    ASSERT_EQ(x[3], 5);
    ASSERT_EQ(x[4], 5);
}
