#ifndef BOARD_H
#define BOARD_H

// includes
#include <ostream>
#include <vector>
#include "Piece.h"

class Board;

typedef std::pair<Piece, std::pair<unsigned, unsigned> > histEnt_t;
typedef std::pair<Board, int> placement_t;

class Board {
    private:
        unsigned _score;
        std::vector<std::vector<bool> > board;
        std::vector<histEnt_t> history;
    public:
        Board(int = 10);
        Board(int, int);

        int place(const Piece&, unsigned, unsigned);
        std::vector<placement_t> place(const Piece&) const;

        bool undo(unsigned = 1);
        histEnt_t last() const;

        unsigned size() const;
        unsigned score() const;
        unsigned height() const;
        unsigned width() const;
        void clear();

        unsigned operator()() const;
        Board operator&(const Board&) const;

        // friends
        friend std::ostream& operator<<(std::ostream&, const Board&);
    private:
        void rowSet(unsigned, bool = true);
        void colSet(unsigned, bool = true);

        bool rowTest(unsigned, bool = true) const;
        bool colTest(unsigned, bool = true) const;

        int update();
};

template <typename A, typename B>
bool comPairBySecondGT(const std::pair<A, B>& left, const std::pair<A, B>& right) {
    return left.second > right.second;
}

#endif
