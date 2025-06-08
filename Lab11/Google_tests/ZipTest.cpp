#include "..\Zip.h"
#include "gtest/gtest.h"
#include <vector>
#include <list>

TEST(ZipTest, listVec) {
    std::list l = {1, 2, 3, 4, 5};
    std::vector<char> v = {'a', 'b', 'c', 'd'};

    std::vector<std::pair<int, char>> ans = {{1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}};

    size_t i = 0;
    for(auto value : zip(l, v)) {
        ASSERT_EQ(value.first, ans[i].first);
        ASSERT_EQ(value.second, ans[i].second);
        ++i;
    }
}

TEST(ZipTest, listVec_upsidedown) {
    std::list l = {1, 2, 3, 4, 5};
    std::vector<char> v = {'a', 'b', 'c', 'd'};

    std::vector<std::pair<int, char>> ans = {{1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}};

    size_t i = 0;
    for(auto value : zip(v, l)) {
        ASSERT_EQ(value.first, ans[i].second);
        ASSERT_EQ(value.second, ans[i].first);
        ++i;
    }
}
