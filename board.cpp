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

Board::TerminalState Board::terminal() {
    if((red | yellow) == 4398046511103) {
        return TerminalState::DRAW;
    }

    for(int row = 0; row < WIDTH; row++) {
        for(int col = 0; col < HEIGHT; col++) {
            int index = row * WIDTH + col;
            if ((red >> index) & 1) {
                // check horizontal
                if (col + 3 < HEIGHT && ((red >> (row * WIDTH + col)) & 1) &&
                    ((red >> (row * WIDTH + col + 1)) & 1) &&
                    ((red >> (row * WIDTH + col + 2)) & 1) &&
                    ((red >> (row * WIDTH + col + 3)) & 1)) {
                    return TerminalState::RED_WON;
                }
                // check vertical
                if (row + 3 < WIDTH && ((red >> index) & 1) &&
                    ((red >> ((row + 1) * WIDTH + col)) & 1) &&
                    ((red >> ((row + 2) * WIDTH + col)) & 1) &&
                    ((red >> ((row + 3) * WIDTH + col)) & 1)) {
                    return TerminalState::RED_WON;
                }
                // check diagonal
                if (col - 3 >= 0 && row + 3 < HEIGHT && ((red >> index) & 1) &&
                    ((red >> ((row + 1) * WIDTH + col - 1)) & 1) &&
                    ((red >> ((row + 2) * WIDTH + col - 2)) & 1) &&
                    ((red >> ((row + 3) * WIDTH + col - 3)) & 1)) {
                    return TerminalState::RED_WON;
                }
                // check diagonal
                if (col + 3 < HEIGHT && row + 3 < WIDTH && ((red >> index) & 1) &&
                    ((red >> ((row + 1) * WIDTH + col + 1)) & 1) &&
                    ((red >> ((row + 2) * WIDTH + col + 2)) & 1) &&
                    ((red >> ((row + 3) * WIDTH + col + 3)) & 1)) {
                    return TerminalState::RED_WON;
                }
            } else if((yellow >> index) & 1){
                // check horizontal
                if (col + 3 < HEIGHT && ((yellow >> (row * WIDTH + col)) & 1) &&
                    ((yellow >> (row * WIDTH + col + 1)) & 1) &&
                    ((yellow >> (row * WIDTH + col + 2)) & 1) &&
                    ((yellow >> (row * WIDTH + col + 3)) & 1)) {
                    return TerminalState::YELLOW_WON;
                }
                // check vertical
                if (row + 3 < WIDTH && ((yellow >> index) & 1) &&
                    ((yellow >> ((row + 1) * WIDTH + col)) & 1) &&
                    ((yellow >> ((row + 2) * WIDTH + col)) & 1) &&
                    ((yellow >> ((row + 3) * WIDTH + col)) & 1)) {
                    return TerminalState::YELLOW_WON;
                }
                // check diagonal
                if (col - 3 >= 0 && row + 3 < HEIGHT && ((yellow >> index) & 1) &&
                    ((yellow >> ((row + 1) * WIDTH + col - 1)) & 1) &&
                    ((yellow >> ((row + 2) * WIDTH + col - 2)) & 1) &&
                    ((yellow >> ((row + 3) * WIDTH + col - 3)) & 1)) {
                    return TerminalState::YELLOW_WON;
                }
                // check diagonal
                if (col + 3 < HEIGHT && row + 3 < WIDTH && ((yellow >> index) & 1) &&
                    ((yellow >> ((row + 1) * WIDTH + col + 1)) & 1) &&
                    ((yellow >> ((row + 2) * WIDTH + col + 2)) & 1) &&
                    ((yellow >> ((row + 3) * WIDTH + col + 3)) & 1)) {
                    return TerminalState::YELLOW_WON;
                }
            }
        }
    }

    return TerminalState::IN_PROGRESS;
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