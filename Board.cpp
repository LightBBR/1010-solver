#include "Board.h"
#include <algorithm>

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
    if (    dim.first  + h > height() ||
            dim.second + w > width())
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

// returns an unsorted list of potential placements
std::vector<placement_t> Board::place(const Piece& p) const {
    std::vector<placement_t> results;
    dim_t dim = p.dim();
    for(unsigned i = 0; i < height() - dim.first + 1; ++i)
        for(unsigned j = 0; j < width() - dim.second + 1; ++j) {
            Board tmp(*this);
            int res = tmp.place(p, i, j);
            if (res > -1) results.push_back(placement_t(tmp, res));
        }
    return results;
}

bool Board::undo(unsigned num) {
    if (num > history.size() || !height()) return false;
    Board tmp(height(), width());
    for(unsigned i = 0; i + num < history.size(); ++i)
        tmp.place(history.at(i).first, history.at(i).second.first, history.at(i).second.second);
    *this = tmp;
    return true;
}

histEnt_t Board::last() const {
    if (!history.size()) return histEnt_t();
    return history.at(history.size()-1);
}

unsigned Board::score() const {
    return _score;
}

unsigned Board::size() const {
    unsigned sz = 0;
    for(unsigned i = 0; i < height(); ++i)
        for(unsigned j = 0; j < width(); ++j)
            if (board.at(i).at(j)) ++sz;
    return sz;
}

unsigned Board::height() const {
    return board.size();
}

unsigned Board::width() const {
    if (!height()) return 0;
    return board.at(0).size();
}

void Board::clear() {
    for(unsigned i = 0; i < height(); ++i)
        for(unsigned j = 0; j < width(); ++j)
            board.at(i).at(j) = false;
}

// TODO: evaluation function
unsigned Board::operator()() const {
    return 100 - size();
}

Board Board::operator&(const Board& b) const {
    Board r(*this);
    for(unsigned i = 0; i < height(); ++i)
        for(unsigned j = 0; j < width(); ++j)
            r.board.at(i).at(j) = board.at(i).at(j) && b.board.at(i).at(j);
    return r;
}

std::ostream& operator<<(std::ostream& out, const Board& b) {
    if (!b.width()) return out;

    for(unsigned i = 0; i < b.width(); ++i) out << "+---";
    out << '+' << std::endl;

    for(unsigned i = 0; i < b.height(); ++i) {
        for(unsigned j = 0; j < b.width(); ++j) {
            out << '|';
            if (b.board.at(i).at(j)) out << " X ";
            else                     out << "   ";
        }
        out << "|" << std::endl;

        for(unsigned j = 0; j < b.width(); ++j) out << "+---";
        out << '+' << std::endl;
    }
    return out;
}

void Board::rowSet(unsigned row, bool fill) {
    board.at(row) = std::vector<bool>(width(), fill);
}

void Board::colSet(unsigned col, bool fill) {
    for(unsigned i = 0; i < height(); ++i) board.at(i).at(col) = fill;
}

bool Board::rowTest(unsigned row, bool fill) const {
    return board.at(row) == std::vector<bool>(width(), fill);
}

bool Board::colTest(unsigned col, bool fill) const {
    for(unsigned i = 0; i < height(); ++i)
        if (board.at(i).at(col) != fill) return false;
    return true;
}

int Board::update() {
    Board r(*this);
    unsigned completed = 0;
    for(unsigned i = 0; i < r.height(); ++i)
        if (r.rowTest(i)) {
            r.rowSet(i, false);
            completed++;
        }
    for(unsigned i = 0; i < width(); ++i)
        if (colTest(i)) {
            colSet(i, false);
            completed++;
        }
    *this = *this & r;
    return 10 * completed * (completed+1) / 2;
}


