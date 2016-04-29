#ifndef PIECE_H
#define PIECE_H

#include <vector>

class Piece {
    private:
        std::vector<std::vector<bool> > piece;
    public:
        Piece(unsigned, unsigned, bool = false);

        std::pair<unsigned, unsigned> dim() const;
        void resize(unsigned, unsigned, bool = false);
        void trim();

        // setters
        bool set();
        bool set(unsigned, unsigned);
        bool clear();
        bool clear(unsigned, unsigned);

        // getters
        std::vector<std::vector<bool> > get() const;
        bool get(unsigned, unsigned) const;

        unsigned size() const;
    private:
        bool setAll(bool);
};

#endif
