#include "CircBuf.h"

#include <iostream>
#include <vector>


int main() {
    CCircularBuffer<int, std::allocator<int>> x (10, 0);
    for (int i = 1; i < 8; ++i) {
        x.pop_front();
    }
    for (int i = 5; i > 0; --i) {
        x.push_back(i);
    }

    for (auto it : x) {
        std::cout<<it<<" ";
    }
    std::cout<<"\n";

    x.insert(x.cbegin() + 5, 9);

    for (auto it : x) {
        std::cout<<it<<" ";
    }
    std::cout<<"\n";

    return 0;
}
