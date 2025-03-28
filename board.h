#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

class Board {
    private:
    uint64_t red = 0;
    uint64_t yellow = 0;
    bool move = false;

    public:
    enum class Tile {
        EMPTY,
        RED,
        YELLOW
    };

    Tile getTile(int row, int col) const;
    
    void dropTile(int col);
};

#endif