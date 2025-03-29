#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

class Board {
    private:
    uint64_t red;
    uint64_t yellow;
    bool move;

    public:

    Board();

    Board(Board &b);

    Board(uint64_t red, uint64_t yellow, bool move);

    enum class Tile {
        EMPTY,
        RED,
        YELLOW
    };

    enum class TerminalState {
        IN_PROGRESS,
        RED_WON,
        YELLOW_WON,
        DRAW
    };

    Tile getTile(int row, int col) const;

    TerminalState terminal();

    int heuristic();

    bool validMove(int col) const;
    
    void dropTile(int col);
};

#endif