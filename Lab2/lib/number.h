#pragma once
#include <cinttypes>
#include <iostream>
#include <string>

const uint8_t ASCII_SYMBOL_CODE_0 = 48;
const uint8_t SIZE_OF_ARRAY = 64; // ceil( 2022 / 32 ); 32 but not 64 because we had to save a number uint32_t + uint_32t
const uint32_t BASE = 1000000000;
const uint8_t BASE_NUMBER_DIGITS = 9;

struct uint2022_t {
    uint32_t arr [SIZE_OF_ARRAY] = {0};
};

static_assert(sizeof(uint2022_t) <= 300, "Size of uint2022_t must be no higher than 300 bytes");

uint2022_t from_uint(uint32_t i);

uint2022_t from_string(const char* buff);

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator>=(const uint2022_t& lhs, const uint2022_t& rhs);

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value);

std::istream& operator>>(std::istream& stream, uint2022_t& value);
