#include "number.h"

#include<cstring>
#include<cstdint>
#include<cmath>

uint2022_t from_uint(uint32_t i) {
    uint2022_t value;
    if (i < BASE) {
        value.arr[0] = i;
    } else {
        value.arr[0] = i % BASE;
        value.arr[1] = int(i / BASE);
    }

    return value;
}

uint2022_t from_string(const char* buff) {
    uint2022_t value;
    uint32_t elem = 0; // element in array of value
    uint8_t elemIndex = 0;
    uint8_t razryad = 0; // razryad [0, 9] of the element
    for (int i = strlen(buff)-1; i>=0; --i)
    {
        if (razryad < BASE_NUMBER_DIGITS) {
          elem = (int(buff[i]) - ASCII_SYMBOL_CODE_0) * pow(10, razryad) + elem;
          ++razryad;
        } else {
            ++i;
            razryad = 0;
            value.arr[elemIndex] = elem;
            ++elemIndex;
            elem = 0;
        }
    }
    if (razryad >= 0) value.arr[elemIndex] = elem;

    return value;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t value;
    uint64_t curr;
    for (int i = 0; i < SIZE_OF_ARRAY; ++i) {
        curr = lhs.arr[i] + rhs.arr[i] + value.arr[i];
        value.arr[i] = curr % BASE;
        if (i < SIZE_OF_ARRAY - 1) {
            value.arr[i + 1] += curr / BASE;
        }
    }

    return value;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t value;
    uint2022_t copy_lhs = lhs;
    int j;
    int first_not_null;
    for (int i = SIZE_OF_ARRAY - 1; i >= 0 ; --i) {
        if(lhs.arr[i] != 0) {
            first_not_null= i;
            break;
        }
    }
    for (int i = 0 ;i < SIZE_OF_ARRAY; ++i)
    {
        if (copy_lhs.arr[i] >= rhs.arr[i]) {
            value.arr[i] = copy_lhs.arr[i] - rhs.arr[i];
        } else {
            j = i + 1;

            while (j <= first_not_null and copy_lhs.arr[first_not_null] == 0) {
                copy_lhs.arr[j] = BASE - 1;
                j++;
            }

            copy_lhs.arr[j] -= 1;
            value.arr[i] = BASE - rhs.arr[i] + lhs.arr[i];
        }
    }

    return value;
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t value;
    uint64_t result;
    uint64_t temp;

    for (int i = 0; i < SIZE_OF_ARRAY; ++i) {
        for (int j = 0; j<SIZE_OF_ARRAY; ++j) {


            result = static_cast<uint64_t>(lhs.arr[i]) * rhs.arr[j];
            int p = 0;
            while (i + j + p < SIZE_OF_ARRAY) {
                if((value.arr[i + j + p] + result) > BASE - 1) {
                    temp = value.arr[i + j + p];
                    value.arr[i + j + p] = (value.arr[i + j + p] + result) % BASE;
                    result = (temp + result) / BASE;
                } else {
                    value.arr[i + j + p] = (value.arr[i + j + p] + result) % BASE;
                    break;
                }
                p++;
            }
        }
    }

    return value;
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t value;
    uint2022_t buffer;
    int first_digit;
    for(int i = SIZE_OF_ARRAY - 1; i >= 0; --i)
    {
        if (lhs.arr[i] != 0) {
            first_digit = i;
            break;
        }
    }
    if (rhs > lhs) {
        return from_uint(0);
    }

    while (first_digit >= 0) {
        while (rhs > buffer and first_digit >= 0) {
            buffer = buffer * from_uint(BASE) + from_uint(lhs.arr[first_digit]);
            --first_digit;
        }
        int l = 0;
        int r = BASE;
        int m;
        while (l <= r) {
            m = (l + r) / 2;
            uint2022_t temp(rhs * from_uint(m));
            if (buffer >= temp)
                l = m + 1;
            else
                r = m - 1;
        }
        value = value * from_uint(BASE) + from_uint(l - 1);
        buffer = buffer - rhs * from_uint(l);
    }

    return value;
}

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs) {
    int f_num_of_digits = 0, s_num_of_digits = 0;
    for(int i = SIZE_OF_ARRAY - 1; i >= 0; --i) {
        if (rhs.arr[i] != 0) {
            f_num_of_digits = i;
            break;
        }
    }
    for(int i = SIZE_OF_ARRAY - 1; i >= 0; --i) {
        if (lhs.arr[i] != 0) {
            s_num_of_digits = i;
            break;
        }
    }
    if (s_num_of_digits > f_num_of_digits){
        return true;
    } else if (s_num_of_digits < f_num_of_digits){
        return false;
    }
    for (int i = f_num_of_digits; i >= 0; --i) {
        if (lhs.arr[i] != rhs.arr[i]) {
            return lhs.arr[i] > rhs.arr[i];
        }

    }
}

bool operator>=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(rhs > lhs);

}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
    for(int i = 0; i < SIZE_OF_ARRAY; ++i)
    {
        if (lhs.arr[i] != rhs.arr[i]) {
            return false;
        }
    }

    return true;
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
    bool not_zero_exist = false;
    for (int i = SIZE_OF_ARRAY - 1; i >= 0; --i) {
        if (value.arr[i]==0) {
            if (!not_zero_exist) continue;
            for (int j = 0; j < BASE_NUMBER_DIGITS; ++j) stream << 0;
        } else {
            not_zero_exist = true;
            stream << value.arr[i];
        }

    }
    if(!not_zero_exist) stream << 0;

    return stream;
}
