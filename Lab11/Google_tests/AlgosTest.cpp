#include "..\Algos.h"
#include "gtest/gtest.h"
#include<vector>

bool Even(int i) {
    return (i % 2 == 0);
}

bool MoreThan(double i) {
    return i > 0.3;
}

bool Eq6(int i) {
    return i == 6;
}

bool AsciiMoreThan(char i) {
    return i >= 'a';
}

bool AsciiEq(char i) {
    return i == 'a';
}

bool Comparator(const std::string& a, const std::string& b) {
    return a < b;
};



TEST(AlgoTest, all_of_int) {
    std::vector<int> v = {1, 3, 5, 2, 4, 6};
    ASSERT_FALSE(CustomAlgo::all_of(v.begin(), v.end(), &Even));
    ASSERT_TRUE(CustomAlgo::all_of(v.begin() + 3, v.end(), &Even));
}

TEST(AlgoTest, all_of_double) {
    std::vector<double> v = {0.1, 0.3, 0.5, 0.7, 4, 6};
    ASSERT_FALSE(CustomAlgo::all_of(v.begin(), v.end(), &MoreThan));
    ASSERT_TRUE(CustomAlgo::all_of(v.begin() + 2, v.end(), &MoreThan));
}

TEST(AlgoTest, all_of_char) {
    std::vector<char> v = {'a', 'b', 'c'};
    ASSERT_TRUE(CustomAlgo::all_of(v.begin(), v.end(), &AsciiMoreThan));
}

TEST(AlgoTest, any_of_int) {
    std::vector<int> v = {1, 3, 5, 2, 4, 6};
    ASSERT_TRUE(CustomAlgo::any_of(v.begin(), v.end(), &Eq6));
    ASSERT_FALSE(CustomAlgo::all_of(v.begin(), v.end() - 1, &Eq6));
}

TEST(AlgoTest, none_of_int) {
    std::vector<int> v = {1, 3, 5, 2, 4, 6};
    ASSERT_TRUE(CustomAlgo::none_of(v.begin(), v.end() - 1, &Eq6));
    ASSERT_FALSE(CustomAlgo::none_of(v.begin(), v.end(), &Eq6));
}

TEST(AlgoTest, one_of_int) {
    std::vector<int> v = {6, 1, 3, 5, 2, 4, 6};
    ASSERT_TRUE(CustomAlgo::one_of(v.begin(), v.end() - 1, &Eq6));
    ASSERT_FALSE(CustomAlgo::one_of(v.begin(), v.end(), &Eq6));
    ASSERT_FALSE(CustomAlgo::one_of(v.begin() + 1, v.end() - 1, &Eq6));
}

TEST(AlgoTest, is_sorted_str) {
    std::vector<std::string> v = {"a", "ab", "ac", "a"};
    ASSERT_TRUE(CustomAlgo::is_sorted(v.begin(), v.end() - 1, &Comparator));
    ASSERT_FALSE(CustomAlgo::is_sorted(v.begin(), v.end(), &Comparator));
}

TEST(AlgoTest, is_partitioned_double) {
    std::vector<char> v = {'a', 'a', 'a', 'b', 'b'};
    ASSERT_TRUE(CustomAlgo::is_partitioned(v.begin(), v.end(), &AsciiEq));
    ASSERT_FALSE(CustomAlgo::is_partitioned(v.begin(), v.end() - 2, &MoreThan));
}

TEST(AlgoTest, find_not_int) {
    std::vector<int> v = {2, 1, 3, 5, 2, 4};
    ASSERT_EQ(CustomAlgo::find_not(v.begin(), v.end(), &Even), v.begin() + 1);

    std::vector<int> v1 = {6, 6, 6, 6};
    ASSERT_EQ(CustomAlgo::find_not(v1.begin(), v1.end(), &Eq6), v1.end());
}

TEST(AlgoTest, find_backward_int) {
    std::vector<int> v = {2, 1, 3, 5, 2, 4, 3, 3, 3};
    ASSERT_EQ(CustomAlgo::find_backward(v.begin(), v.end(), &Even), v.end() - 4);

    std::vector<int> v1 = {1, 1, 1, 1};
    ASSERT_EQ(CustomAlgo::find_backward(v1.begin(), v1.end(), &Eq6), v1.begin() - 1);
}

TEST(AlgoTest, is_palindrome_int) {
    std::vector<int> v = {1, 3, 8, 10, 2, 5, 3};
    ASSERT_TRUE(CustomAlgo::is_palindrome(v.begin(), v.end(), &Even));

    std::vector<char> v1 = {'a', 'b', 'b'};
    ASSERT_FALSE(CustomAlgo::is_palindrome(v1.begin(), v1.end(), &AsciiEq));
}


