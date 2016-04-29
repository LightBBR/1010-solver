#ifndef BOARD_H
#define BOARD_H

// includes
#include <ostream>
#include <vector>
#include "Piece.h"

class Board {
    private:
        unsigned _score;
        std::vector<std::vector<bool> > board;
        std::vector<std::pair<Piece, std::pair<unsigned, unsigned> > > history;
    public:
        Board(int = 10);
        Board(int, int);

        int place(const Piece&, unsigned, unsigned);
        std::vector<std::pair<Board, int> > place(const Piece&) const;
        std::pair<Board, int> bestPlace(const Piece& p) const;

        bool undo(unsigned = 1);
        std::pair<Piece, std::pair<unsigned, unsigned> > last() const;

        unsigned score() const;
        unsigned size() const;
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

#endif
