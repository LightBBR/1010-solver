#include "Board.h"
#include <algorithm>

/*
#include <iostream>
using namespace std;
// */

Board::Board(int size) : _score(0), board(size, std::vector<bool>(size, false)) {
}

Board::Board(int height, int width) : _score(0), board(height, std::vector<bool>(width, false)) {
}

// attempt to place a piece
// return: -1 - failed to place
//        >-1 - the number of points gained by placing
//              at the given location
int Board::place(const Piece& p, unsigned h, unsigned w) {
    std::pair<unsigned, unsigned> dim = p.dim();
    if (!this->board.size() ||
            dim.first  + h > this->board.at(0).size() ||
            dim.second + w > this->board.size())
        return -1;

    for(unsigned i = 0; i < dim.first; ++i)
        for(unsigned j = 0; j < dim.second; ++j)
            if (p.get(i, j) && this->board.at(h+i).at(w+j)) return -1;

    for(unsigned i = 0; i < dim.first; ++i)
        for(unsigned j = 0; j < dim.second; ++j)
            this->board.at(h+i).at(w+j) = this->board.at(h+i).at(w+j) || p.get(i,j);

    this->history.push_back(std::pair<Piece, std::pair<unsigned, unsigned> >(p, std::pair<unsigned, unsigned>(h, w)));
    unsigned oldScore = this->_score;
    this->_score += this->update() + p.size();

    return oldScore - this->update();
}

template <typename A, typename B>
bool comPairBySecondGT(const std::pair<A, B>& left, const std::pair<A, B>& right) {
    return left.second > right.second;
}

// returns a sorted list of potential placements
std::vector<std::pair<Board, int> > Board::place(const Piece& p) const {
    std::vector<std::pair<Board, int> > results;
    std::pair<unsigned, unsigned> dim = p.dim();
    for(unsigned i = 0; i < this->board.size() - dim.first + 1; ++i)
        for(unsigned j = 0; j < this->board.at(i).size() - dim.second + 1; ++j) {
            Board tmp(*this);
            int res = tmp.place(p, i, j);
            if (res > -1) results.push_back(std::pair<Board, int>(tmp, res));
        }
    sort(results.begin(), results.end(), comPairBySecondGT<Board, int>);
    return results;
}

std::pair<Board, int> Board::bestPlace(const Piece& p) const {
    std::vector<std::pair<Board, int> > results(this->place(p));

    if (!results.size()) return std::pair<Board, int>(*this, -1);

    unsigned i = 1;
    while (i < results.size() && results.at(0).second == results.at(i).second) ++i;
    results.resize(i);

    std::vector<std::pair<std::pair<Board, int>, unsigned> > best;
    for(unsigned i = 0; i < results.size(); ++i)
        best.push_back(std::pair<std::pair<Board, int>, unsigned>(results.at(i), results.at(i).first()));
    sort(best.begin(), best.end(), comPairBySecondGT<std::pair<Board, int>, unsigned>);

    return best.at(0).first;
}

bool Board::undo(unsigned num) {
    if (!num || !this->board.size()) return false;
    Board tmp(this->board.size(), this->board.at(0).size());
    if (static_cast<signed>(this->history.size()) - static_cast<signed>(num) < 0) return false;
    for(unsigned i = 0; i < this->history.size() - num; ++i)
        tmp.place(this->history.at(i).first, this->history.at(i).second.first, this->history.at(i).second.second);
    *this = tmp;
    return true;
}

std::pair<Piece, std::pair<unsigned, unsigned> > Board::last() const {
    return this->history.at(this->history.size()-1);
}

unsigned Board::score() const {
    return this->_score;
}

unsigned Board::size() const {
    unsigned sz = 0;
    for(unsigned i = 0; i < this->board.size(); ++i)
        for(unsigned j = 0; j < this->board.at(i).size(); ++j)
            if (this->board.at(i).at(j)) ++sz;
    return sz;
}

void Board::clear() {
    for(unsigned i = 0; i < this->board.size(); ++i)
        for(unsigned j = 0; j < this->board.at(i).size(); ++j)
            this->board.at(i).at(j) = false;
}

// TODO: evaluation function
unsigned Board::operator()() const {
    return 100 - this->size();
}

Board Board::operator&(const Board& b) const {
    Board r(*this);
    for(unsigned i = 0; i < this->board.size(); ++i)
        for(unsigned j = 0; j < this->board.at(i).size(); ++j)
            r.board.at(i).at(j) = this->board.at(i).at(j) & b.board.at(i).at(j);
    return r;
}

std::ostream& operator<<(std::ostream& out, const Board& b) {
    if (!b.board.size()) return out;

    for(unsigned i = 0; i < b.board.at(0).size(); ++i) out << "+---";
    out << '+' << std::endl;

    for(unsigned i = 0; i < b.board.size(); ++i) {
        for(unsigned j = 0; j < b.board.at(i).size(); ++j) {
            out << '|';
            if (b.board.at(i).at(j)) out << " X ";
            else                     out << "   ";
        }
        out << "|" << std::endl;

        for(unsigned j = 0; j < b.board.at(i).size(); ++j) out << "+---";
        out << '+' << std::endl;
    }
    return out;
}

void Board::rowSet(unsigned row, bool fill) {
    this->board.at(row) = std::vector<bool>(this->board.at(row).size(), fill);
}

void Board::colSet(unsigned col, bool fill) {
    for(unsigned i = 0; i < this->board.size(); ++i) this->board.at(i).at(col) = fill;
}

bool Board::rowTest(unsigned row, bool fill) const {
    return this->board.at(row) == std::vector<bool>(this->board.at(row).size(), fill);
}

bool Board::colTest(unsigned col, bool fill) const {
    bool diff = false;
    for(unsigned i = 0; i < this->board.size(); ++i)
        if (this->board.at(i).at(col) != fill) diff = true;
    return !diff;
}

int Board::update() {
    Board r(*this);
    unsigned completed = 0;
    for(unsigned i = 0; i < r.board.size(); ++i)
        if (r.rowTest(i)) {
            r.rowSet(i, false);
            completed++;
        }
    for(unsigned i = 0; i < this->board.size(); ++i)
        if (this->colTest(i)) {
            this->colSet(i, false);
            completed++;
        }
    *this = *this & r;
    return 10 * completed * (completed+1) / 2;
}


