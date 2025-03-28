#include <cstdint>
#include "board.h"

static const unsigned int WIDTH = 7;
static const unsigned int HEIGHT = 6;

Board::Tile Board::getTile(int row, int col) const {
    int index = row * WIDTH + col;
    if ((red >> index) & 1) {
        return Tile::RED;
    } else if ((yellow >> index) & 1) {
        return Tile::YELLOW;
    } else {
        return Tile::EMPTY;
    }
}

// Implement the dropTile method
void Board::dropTile(int col) {
    if (col < 0 || col >= WIDTH) {
        return;
    }

    for (int row = HEIGHT - 1; row >= 0; --row) {
        int index = row * WIDTH + col;
        if (getTile(row, col) != Tile::EMPTY) {
            continue;
        }

        if (move) {
            red |= (1ULL << index);
        } else {
            yellow |= (1ULL << index);
        }
        move = !move;
        break;
    }
}