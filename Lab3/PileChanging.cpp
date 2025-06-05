#include <iostream>
#include <deque>
#include <cmath>

void PushRight(std::deque<std::deque<int64_t>>& matrix, const uint16_t length){
    for (int i = 0; i < length; i++) {
        matrix[i].push_back(0);
    }
}

void PushLeft(std::deque<std::deque<int64_t>>& matrix, const uint16_t length){
    for (int i = 0; i < length; i++) {
        matrix[i].push_front(0);
    }
}

void PushUp(std::deque<std::deque<int64_t>>& matrix, const uint16_t width){
    std::deque<int64_t> a(width, 0);
    matrix.push_front(a);
}

void PushDown(std::deque<std::deque<int64_t>>& matrix, const uint16_t width){
    std::deque<int64_t> a(width, 0);
    matrix.push_back(a);
}

bool PileDrop(uint16_t& length, uint16_t& width, std::deque<std::deque<int64_t>>& pile) {
    bool fl = false;
    for (size_t i = 0; i < length; i++) {
        for (size_t j = 0; j < width; j++) {
            if (pile[i][j] > 0) {

                if (pile[i][j] > 3) {

                    if (j == width - 1) {
                        PushRight(pile, length);
                        width++;
                    }
                    if (j == 0) {
                        PushLeft(pile, length);
                        width++;
                        j = 1;
                    }
                    if (i == 0) {
                        PushUp(pile, width);
                        length++;
                        i = 1;
                    }
                    if (i == length - 1) {
                        PushDown(pile, width);
                        length++;
                    }
                    pile[i - 1][j] += 1;

                    if (pile[i + 1][j] < 0) pile[i + 1][j] -= 1;
                    else if (pile[i + 1][j] < 4) pile[i + 1][j] = (pile[i + 1][j] + 1) * (-1); // cell < 0 if ve shouldn't change it on current iteration
                    else pile[i + 1][j] += 1;

                    if (pile[i][j + 1] < 0) pile[i][j + 1] -= 1;
                    else if (pile[i][j + 1] < 4) pile[i][j + 1] = (pile[i][j + 1] + 1) * (-1);
                    else pile[i][j + 1] += 1;

                    pile[i][j - 1] += 1;

                    pile[i][j] -= 4;

                    if (abs(pile[i - 1][j]) > 3 or abs(pile[i + 1][j]) > 3 or abs(pile[i][j + 1]) > 3 or
                        abs(pile[i][j - 1]) > 3 or
                        abs(pile[i][j]) > 3)
                        fl = true;
                }
            } else {
                pile[i][j] = pile[i][j] * (-1);

                if ((i > 0 and abs(pile[i - 1][j]) > 3) or (i < length - 1 and abs(pile[i + 1][j]) > 3) or
                    (j < width - 1 and abs(pile[i][j + 1]) > 3) or (j > 0 and abs(pile[i][j - 1]) > 3) or
                    abs(pile[i][j]) > 3)
                    fl = true;
            }
        }

    }

    return fl;
}
