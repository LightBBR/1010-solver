#ifndef PIECE_H
#define PIECE_H

#include <ostream>
#include <vector>

class Piece;

typedef std::pair<unsigned, unsigned> dim_t;

class Piece {
    private:
        std::vector<std::vector<bool> > piece;
    public:
        Piece(unsigned, unsigned, bool = false);
        Piece();

        dim_t dim() const;
        void resize(unsigned, unsigned, bool = false);
        void trim();

        bool set();
        bool set(unsigned, unsigned);
        bool clear();
        bool clear(unsigned, unsigned);

        std::vector<std::vector<bool> > get() const;
        bool get(unsigned, unsigned) const;

        unsigned size() const;
    private:
        bool setAll(bool);
};

std::ostream& operator<<(std::ostream& out, const Piece& p);

#endif
