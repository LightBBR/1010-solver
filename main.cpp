#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>

#include "Piece.h"
#include "Board.h"

// global pieces
Piece _1x1(1, 1, true);
Piece _1x2(1, 2, true);
Piece _1x3(1, 3, true);
Piece _1x4(1, 4, true);
Piece _1x5(1, 5, true);
Piece _2x1(2, 1, true);
Piece _2x2(2, 2, true);
Piece _3x1(3, 1, true);
Piece _3x3(3, 3, true);
Piece _4x1(4, 1, true);
Piece _5x1(5, 1, true);


void clear() {
    std::cout << "\ec";
}

void pause() {
    std::string trash;
    getline(std::cin, trash);
}

Piece randomPiece() {
    Piece l000(_2x2); l000.clear(0, 1);
    Piece l090(_2x2); l090.clear(0, 0);
    Piece l180(_2x2); l180.clear(1, 0);
    Piece l270(_2x2); l270.clear(1, 1);

    Piece L000(_3x3); L000.clear(0, 1); L000.clear(0, 2); L000.clear(1, 1); L000.clear(1, 2);
    Piece L090(_3x3); L090.clear(0, 0); L090.clear(0, 1); L090.clear(1, 0); L090.clear(1, 1);
    Piece L180(_3x3); L180.clear(1, 0); L180.clear(1, 1); L180.clear(2, 0); L180.clear(2, 1);
    Piece L270(_3x3); L270.clear(1, 1); L270.clear(1, 2); L270.clear(2, 1); L270.clear(2, 2);

    switch(rand() % 19) {
        case 0:
            return _1x1;
            break;
        case 1:
            return _1x2;
            break;
        case 2:
            return _1x3;
            break;
        case 3:
            return _1x4;
            break;
        case 4:
            return _1x5;
            break;
        case 5:
            return _2x1;
            break;
        case 6:
            return _2x2;
            break;
        case 7:
            return _3x1;
            break;
        case 8:
            return _3x3;
            break;
        case 9:
            return _4x1;
            break;
        case 10:
            return _5x1;
            break;
        case 11:
            return l000;
            break;
        case 12:
            return l090;
            break;
        case 13:
            return l180;
            break;
        case 14:
            return l270;
            break;
        case 15:
            return L000;
            break;
        case 16:
            return L090;
            break;
        case 17:
            return L180;
            break;
        case 18:
            return L270;
            break;
        default:
            return Piece(0, 0);
    }
}

void randomize(Board& b, unsigned tries = 1000) {
    for(unsigned i = 0; i < tries; ++i)
        b.place(randomPiece(), rand() % 10, rand() % 10);
}

void test_3x3() {
    Board b;
    b.place(_3x3, 0, 0);
    b.place(_3x3, 0, 3);
    b.place(_3x1, 0, 6);
    b.place(_1x1, 0, 9);
    clear(); std::cout << b; pause();

    std::vector<std::pair<Board, int> > results = b.place(_3x3);

    for(unsigned i = 0; i < results.size(); ++i) {
        clear();
        std::cout << results.at(i).first << results.at(i).second << std::endl
            << i;
        if (i + 1 == results.size()) std::cout << " (last)";
        std::cout << std::endl;
        pause();
    }

    clear();
}

void test_1x4() {
    Board b;
    b.place(_3x3, 0, 0);
    b.place(_3x3, 0, 3);
    clear(); std::cout << b; pause();

    std::vector<std::pair<Board, int> > results = b.place(_4x1);

    for(unsigned i = 0; i < results.size(); ++i) {
        clear();
        std::cout << results.at(i).first << results.at(i).second << std::endl
            << i;
        if (i + 1 == results.size()) std::cout << " (last)";
        std::cout << std::endl;
        pause();
    }

    clear();
}

void test_randomize() {
    unsigned tries, reps;
    std::cout << "Enter tries: ";
    std::cin  >> tries;
    std::cout << "Enter reps: ";
    std::cin  >> reps;
    for(unsigned i = 0; i < reps; ++i) {
        Board b;
        randomize(b, tries);
        clear(); std::cout << b; pause();
    }
}

void test_history() {
    Board b;
    randomize(b);
    clear(); std::cout << b; pause();
    while (b.undo()) {
        clear(); std::cout << b; pause();
    }
    clear();
}

int main() {
    srand(time(0));
    return 0;
}


