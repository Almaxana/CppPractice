#include <lib/number.h>
#include <iostream>

int main() {
    uint2022_t a = from_uint(2147483647);
    uint2022_t b = from_string("2147483647");
    bool x = a == b;
    std::cout<< x;

    return 0;
}
