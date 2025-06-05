#include "bmp.cpp"
#include "parser.cpp"
#include "PileChanging.cpp"

#include <deque>
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    uint16_t length = 0;
    uint16_t width = 0;
    std::string inpt;
    std::string outpt;
    uint64_t iter = 50000;
    uint64_t freq = 0;

    parserFunction(argc, argv, length, width, inpt, outpt, iter, freq);
    if (length == 0 || width == 0) {
        std::cerr << "Length and width must be > 0"<< std::endl;
        return 0;
    }

    std::deque<std::deque<int64_t>> pile;
    std::deque<int64_t> a (width, 0);
    for (size_t i = 0; i < length; i++){
        pile.push_back(a);
    }

    std::ifstream f;
    std::string s;
    uint16_t x;
    uint16_t y;
    uint64_t value;
    f.open(inpt);
    if (!f.is_open()) {
        std::cerr << "File can't be opened: " << inpt << std::endl;
        return 0;
    }
    if (f.fail()) {
        std::cerr << "Error while opening file: " << inpt << std::endl;
        return 0;
    }
    while (f >> s) {
        x = stoi(s);
        if (x > width) {
            std::cerr << "Coordinate x = " << x << " is out of the width" << std::endl;
            return 0;
        }

        f >> s;
        y = stoi(s);
        if (y > length) {
            std::cerr << "Coordinate y = " << y << " is out of the length" << std::endl;
            return 0;
        }

        f >> s;
        value = stoi(s);

        pile[x][y] = value;
    }

    bool fl = true; // fl is true if unstable cells exist
    uint64_t count_iter = 0;
    while (fl and count_iter < iter) {

        fl = PileDrop(length, width, pile);

        count_iter++;
        if (freq!=0 and count_iter % freq == 0) {
            bmp(length, width, pile, outpt, count_iter / freq);
        }
    }

    if (freq == 0) bmp(length, width, pile, outpt, 0);

    return 0;
}
