#include "Piece.h"
#include <vector>

Piece::Piece(unsigned height, unsigned width, bool init) : piece(height, std::vector<bool>(width, init)) {
}

Piece::Piece() : piece() {
}

// return: height, width
dim_t Piece::dim() const {
    if (piece.size())
        return dim_t(piece.size(), piece.at(0).size());
    else
        return dim_t(0, 0);
}

void Piece::resize(unsigned height, unsigned width, bool fill) {
    piece.resize(height);
    for(unsigned i = 0; i < piece.size(); ++i) piece.at(i).resize(width, fill);
}

void Piece::trim() {
    int maxWidth = 0;
    int maxHeight = 0;
    for(unsigned i = 0; i < piece.size(); ++i) {
        int width = 0;
        for(unsigned j = 0; j < piece.at(i).size(); ++j)
            if (piece.at(i).at(j)) width = j + 1;
        if (width) {
            if (width > maxWidth) maxWidth = width;
            maxHeight = i + 1;
        }
    }
    resize(maxHeight, maxWidth);
}

// set all spots to specified fill
bool Piece::setAll(bool fill) {
    bool change = false;
    if (!piece.size()) return change;
    for(unsigned i = 0; i < piece.size(); ++i)
        for(unsigned j = 0; j < piece.at(i).size(); ++j) {
            if (piece.at(i).at(j) != fill) change = true;
            piece.at(i).at(j) = fill;
        }
    return change;
}

// set all spots to filled
// return: wheter or not a change occurred
bool Piece::set() {
    return setAll(true);
}

// set specific spot to filled
// return: wheter or not a change occurred
bool Piece::set(unsigned h, unsigned w) {
    bool orig = piece.at(h).at(w);
    piece.at(h).at(w) = true;
    return !orig;
}

// clear all spots to empty
// return: wheter or not a change occurred
bool Piece::clear() {
    return setAll(false);
}

// clear specific spot to empty
// return: wheter or not a change occurred
bool Piece::clear(unsigned h, unsigned w) {
    bool orig = piece.at(h).at(w);
    piece.at(h).at(w) = false;
    return orig;
}

// returns entire piece
std::vector<std::vector<bool> > Piece::get() const {
    return piece;
}

// returns fill of specific spot
bool Piece::get(unsigned h, unsigned w) const {
    return piece.at(h).at(w);
}


unsigned Piece::size() const {
    unsigned sz = 0;
    for(unsigned i = 0; i < piece.size(); ++i)
        for(unsigned j = 0; j < piece.at(i).size(); ++j)
            if (piece.at(i).at(j)) ++sz;
    return sz;
}

std::ostream& operator<<(std::ostream& out, const Piece& p) {
    dim_t dim = p.dim();
    if (!dim.first) return out;

    for(unsigned i = 0; i < dim.second; ++i) out << "+---";
    out << '+' << std::endl;

    for(unsigned i = 0; i < dim.first; ++i) {
        for(unsigned j = 0; j < dim.second; ++j) {
            out << '|';
            if (p.get(i, j)) out << " X ";
            else             out << "   ";
        }
        out << "|" << std::endl;

        for(unsigned j = 0; j < dim.second; ++j) out << "+---";
        out << '+' << std::endl;
    }
    return out;
}


