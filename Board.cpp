#include "Board.h"
#include <algorithm>


template <typename A, typename B>
bool comPairBySecondGT(const std::pair<A, B>& left, const std::pair<A, B>& right) {
    return left.second > right.second;
}

Board::Board(int size) : _score(0), board(size, std::vector<bool>(size, false)) {
}

Board::Board(int height, int width) : _score(0), board(height, std::vector<bool>(width, false)) {
}

// attempt to place a piece
// return: -1 - failed to place
//        >-1 - the number of points gained by placing
//              at the given location
int Board::place(const Piece& p, unsigned h, unsigned w) {
    dim_t dim = p.dim();
    if (!board.size() ||
            dim.first  + h > board.at(0).size() ||
            dim.second + w > board.size())
        return -1;

    for(unsigned i = 0; i < dim.first; ++i)
        for(unsigned j = 0; j < dim.second; ++j)
            if (p.get(i, j) && board.at(h+i).at(w+j)) return -1;

    for(unsigned i = 0; i < dim.first; ++i)
        for(unsigned j = 0; j < dim.second; ++j)
            board.at(h+i).at(w+j) = board.at(h+i).at(w+j) || p.get(i,j);

    history.push_back(histEnt_t(p, dim_t(h, w)));
    unsigned oldScore = _score;
    _score += update() + p.size();

    return oldScore - update();
}

// returns a sorted list of potential placements
std::vector<placement_t> Board::place(const Piece& p) const {
    std::vector<placement_t> results;
    dim_t dim = p.dim();
    for(unsigned i = 0; i < board.size() - dim.first + 1; ++i)
        for(unsigned j = 0; j < board.at(i).size() - dim.second + 1; ++j) {
            Board tmp(*this);
            int res = tmp.place(p, i, j);
            if (res > -1) results.push_back(placement_t(tmp, res));
        }
    sort(results.begin(), results.end(), comPairBySecondGT<Board, int>);
    return results;
}

bool Board::undo(unsigned num) {
    if (!num || !board.size()) return false;
    Board tmp(board.size(), board.at(0).size());
    if (static_cast<signed>(history.size()) - static_cast<signed>(num) < 0) return false;
    for(unsigned i = 0; i < history.size() - num; ++i)
        tmp.place(history.at(i).first, history.at(i).second.first, history.at(i).second.second);
    *this = tmp;
    return true;
}

histEnt_t Board::last() const {
    return history.at(history.size()-1);
}

unsigned Board::score() const {
    return _score;
}

unsigned Board::size() const {
    unsigned sz = 0;
    for(unsigned i = 0; i < board.size(); ++i)
        for(unsigned j = 0; j < board.at(i).size(); ++j)
            if (board.at(i).at(j)) ++sz;
    return sz;
}

unsigned Board::height() const {
    return board.size();
}

unsigned Board::width() const {
    if (!board.size()) return 0;
    return board.at(0).size();
}

void Board::clear() {
    for(unsigned i = 0; i < board.size(); ++i)
        for(unsigned j = 0; j < board.at(i).size(); ++j)
            board.at(i).at(j) = false;
}

// TODO: evaluation function
unsigned Board::operator()() const {
    return 100 - size();
}

Board Board::operator&(const Board& b) const {
    Board r(*this);
    for(unsigned i = 0; i < board.size(); ++i)
        for(unsigned j = 0; j < board.at(i).size(); ++j)
            r.board.at(i).at(j) = board.at(i).at(j) & b.board.at(i).at(j);
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
    board.at(row) = std::vector<bool>(board.at(row).size(), fill);
}

void Board::colSet(unsigned col, bool fill) {
    for(unsigned i = 0; i < board.size(); ++i) board.at(i).at(col) = fill;
}

bool Board::rowTest(unsigned row, bool fill) const {
    return board.at(row) == std::vector<bool>(board.at(row).size(), fill);
}

bool Board::colTest(unsigned col, bool fill) const {
    bool diff = false;
    for(unsigned i = 0; i < board.size(); ++i)
        if (board.at(i).at(col) != fill) diff = true;
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
    for(unsigned i = 0; i < board.size(); ++i)
        if (colTest(i)) {
            colSet(i, false);
            completed++;
        }
    *this = *this & r;
    return 10 * completed * (completed+1) / 2;
}


