#include "Xrange.h"

#include <iostream>

int main() {
    auto x = xrange(4, 7);
    for (auto it : x) {
        std::cout<<it<<" ";
    }

    return 0;
}