#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>

#include "Piece.h"
#include "Board.h"

void clear() {
    return;
    std::cout << "\ec";
}

void pause() {
    return;
    std::string trash;
    getline(std::cin, trash);
}

Piece randomPiece() {
    //return Piece(3, 3, true);
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

    Piece l000(_2x2); l000.clear(0, 1);
    Piece l090(_2x2); l090.clear(0, 0);
    Piece l180(_2x2); l180.clear(1, 0);
    Piece l270(_2x2); l270.clear(1, 1);

    Piece L000(_3x3); L000.clear(0, 1); L000.clear(0, 2); L000.clear(1, 1); L000.clear(1, 2);
    Piece L090(_3x3); L090.clear(0, 0); L090.clear(0, 1); L090.clear(1, 0); L090.clear(1, 1);
    Piece L180(_3x3); L180.clear(1, 0); L180.clear(1, 1); L180.clear(2, 0); L180.clear(2, 1);
    Piece L270(_3x3); L270.clear(1, 1); L270.clear(1, 2); L270.clear(2, 1); L270.clear(2, 2);

    switch(rand() % 11) {
    //switch(rand() % 19) {
        case 0:  return _1x1;
        case 1:  return _1x2;
        case 2:  return _1x3;
        case 3:  return _1x4;
        case 4:  return _1x5;
        case 5:  return _2x1;
        case 6:  return _2x2;
        case 7:  return _3x1;
        case 8:  return _3x3;
        case 9:  return _4x1;
        case 10: return _5x1;
        case 11: return l000;
        case 12: return l090;
        case 13: return l180;
        case 14: return l270;
        case 15: return L000;
        case 16: return L090;
        case 17: return L180;
        case 18: return L270;
        default: return Piece();
    }
}

void randomize(Board& b, unsigned tries = 1000) {
    for(unsigned i = 0; i < tries; ++i)
        b.place(randomPiece(), rand() % b.height(), rand() % b.width());
}

void test_history() {
    Board b(15, 20);
    randomize(b);
    do {
        clear();
        histEnt_t h = b.last();
        std::cout << b;
        std::cout << "About to remove piece from (" << h.second.first << ", " << h.second.second << ')' << std::endl;
        pause();
    } while (b.undo());
}

void trim(std::vector<placement_t>& v) {
    //sort(v.begin(), v.end(), comPairBySecondGT<Board, int>);
    v.size();
}

#define CUTOFF 10000000

#define PLACE(SOFAR, PIECE)\
    for(unsigned i = 0; bp ## SOFAR ## PIECE.size() < CUTOFF && i < bp ## SOFAR.size(); ++i) {\
        std::vector<placement_t> tmp = bp ## SOFAR[i].first.place(p ## PIECE);\
        trim(tmp);\
        for(unsigned j = 0; j < tmp.size(); ++j) {\
            tmp[j].second += bp ## SOFAR [i].second;\
        }\
        bp ## SOFAR ## PIECE.insert(bp ## SOFAR ## PIECE.end(), tmp.begin(), tmp.end());\
    }

void brute() {
    Board b;
    std::vector<placement_t> best;

    Piece p1, p2, p3;

    do {
        clear();
        best.clear();

        std::cout << (p1 = randomPiece());
        std::cout << (p2 = randomPiece());
        std::cout << (p3 = randomPiece());

        std::vector<placement_t> bp1 = b.place(p1);
        std::vector<placement_t> bp12;
        std::vector<placement_t> bp13;
        std::vector<placement_t> bp123;
        std::vector<placement_t> bp132;

        std::vector<placement_t> bp2 = b.place(p2);
        std::vector<placement_t> bp21;
        std::vector<placement_t> bp23;
        std::vector<placement_t> bp213;
        std::vector<placement_t> bp231;

        std::vector<placement_t> bp3 = b.place(p3);
        std::vector<placement_t> bp31;
        std::vector<placement_t> bp32;
        std::vector<placement_t> bp312;
        std::vector<placement_t> bp321;

        PLACE(1, 2);
        PLACE(1, 3);
        PLACE(12, 3);
        PLACE(13, 2);
        PLACE(2, 1);
        PLACE(2, 3);
        PLACE(21, 3);
        PLACE(23, 1);
        PLACE(3, 1);
        PLACE(3, 2);
        PLACE(31, 2);
        PLACE(32, 1);

        sort(bp123.begin(), bp123.end(), comPairBySecondGT<Board, int>);
        sort(bp132.begin(), bp132.end(), comPairBySecondGT<Board, int>);
        sort(bp213.begin(), bp213.end(), comPairBySecondGT<Board, int>);
        sort(bp231.begin(), bp231.end(), comPairBySecondGT<Board, int>);
        sort(bp312.begin(), bp312.end(), comPairBySecondGT<Board, int>);
        sort(bp321.begin(), bp321.end(), comPairBySecondGT<Board, int>);

        if (bp123.size()) best.push_back(bp123[0]);
        else std::cout << "No paths through 123" << std::endl;
        if (bp132.size()) best.push_back(bp132[0]);
        else std::cout << "No paths through 132" << std::endl;
        if (bp213.size()) best.push_back(bp213[0]);
        else std::cout << "No paths through 213" << std::endl;
        if (bp231.size()) best.push_back(bp231[0]);
        else std::cout << "No paths through 231" << std::endl;
        if (bp312.size()) best.push_back(bp312[0]);
        else std::cout << "No paths through 312" << std::endl;
        if (bp321.size()) best.push_back(bp321[0]);
        else std::cout << "No paths through 321" << std::endl;

        sort(best.begin(), best.end(), comPairBySecondGT<Board, int>);

        if (best.size()) {
            b = best.at(0).first;
            std::cout << b;
        } else {
            std::cout << "FAILED." << std::endl
                << "Final score: " << b.score() << std::endl;
        }
        pause();
        std::cout << std::string(80, '=') << std::endl;
    } while (best.size());
}

int main() {
    srand(time(0));

    brute();

    return 0;
}


