#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <set>
#include <fstream>

#include "MyAllocator.h"


// functions for making statistic graphs

template <typename T>
void testList(const std::string& outFilePath, PoolAllocator<T> allocator) {
    std::ofstream fout(outFilePath);
    std::list<int, PoolAllocator<int>> m_list(allocator);
    std::list<int> stand_list;

    auto begin = std::chrono::steady_clock::now();
    for (size_t i = 1; i < 100000; ++i) {
        fout<<i<<"  ";
        stand_list.push_back(i);
        auto end = std::chrono::steady_clock::now();
        double elapsed_ns = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        fout<<elapsed_ns<<"\n";
    }

    fout<<"\n\n\n\n\n";

    begin = std::chrono::steady_clock::now();
    for (size_t i = 1; i < 100000; ++i) {
        fout<<i<<"  ";
        m_list.push_back(i);
        auto end = std::chrono::steady_clock::now();
        double elapsed_ns = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        fout << elapsed_ns << "\n";
    }
}


template <typename T>
void testSet(const std::string& outFilePath, PoolAllocator<T> allocator) {
    std::ofstream fout(outFilePath);
    std::set<int, std::less<int>, PoolAllocator<int>> m_set(allocator);
    std::set<int> stand_set;

    auto begin = std::chrono::steady_clock::now();
    for (size_t i = 1; i < 100000; ++i) {
        fout<<i<<"  ";
        stand_set.insert(i);
        auto end = std::chrono::steady_clock::now();
        double elapsed_ns = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        fout<<elapsed_ns<<"\n";
    }

    fout<<"\n\n\n\n\n";

    begin = std::chrono::steady_clock::now();
    for (size_t i = 1; i < 100000; ++i) {
        fout<<i<<"  ";
        m_set.insert(i);
        auto end = std::chrono::steady_clock::now();
        double elapsed_ns = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        fout << elapsed_ns << "\n";
    }
}


template <typename T>
void testVector(const std::string& outFilePath, PoolAllocator<T> allocator) {
    std::ofstream fout(outFilePath);
    std::vector<int, PoolAllocator<int>> m_vec(allocator);
    std::vector<int> stand_vec;

    auto begin = std::chrono::steady_clock::now();
    for (size_t i = 1; i < 100000; ++i) {
        fout<<i<<"  ";
        stand_vec.push_back(i);
        auto end = std::chrono::steady_clock::now();
        double elapsed_ns = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        fout<<elapsed_ns<<"\n";
    }

    fout<<"\n\n\n\n\n";

    begin = std::chrono::steady_clock::now();
    for (size_t i = 1; i < 100000; ++i) {
        fout<<i<<"  ";
        m_vec.push_back(i);
        auto end = std::chrono::steady_clock::now();
        double elapsed_ns = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        fout << elapsed_ns << "\n";
    }
}


int main() {
    std::vector<Pool> pools;
    Pool pool0 (100000, 20);
    Pool pool1 (100000000, 50);
    //Pool pool2 (100000, 1000);
    /*Pool pool3 (100000, 10000);
    Pool pool4 (10000000, 1000000);*/

    pools.push_back(pool0);
    pools.push_back(pool1);
    //pools.push_back(pool2);
    /*pools.push_back(pool3);
    pools.push_back(pool4);*/


    PoolAllocator<int> my_alloc(pools);

    std::list<int, PoolAllocator<int>> m_list(my_alloc);
    std::list<int> stand_list;


    auto begin = std::chrono::steady_clock::now();
    for (size_t i = 1; i < 100000; ++i) {
        stand_list.push_back(i);
    }
    auto end = std::chrono::steady_clock::now();
    double elapsed_ns = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout<<"standart time   "<<elapsed_ns<<"\n";


    begin = std::chrono::steady_clock::now();
    for (size_t i = 1; i < 100000; ++i) {
        m_list.push_back(i);
    }
    end = std::chrono::steady_clock::now();
    elapsed_ns = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout<<"time   "<<elapsed_ns<<"\n";
}
