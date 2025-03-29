#include <cstdint>
#include "board.h"

static const unsigned int WIDTH = 7;
static const unsigned int HEIGHT = 6;

Board::Board() {
    this->red = 0;
    this->yellow = 0;
    this->move = true;
}

Board::Board(Board &b) {
    this->red = b.red;
    this->yellow = b.yellow;
    this->move = b.move;
}

Board::Board(uint64_t red, uint64_t yellow, bool move) {
    this->red = red;
    this->yellow = yellow;
    this->move = move;
}

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

    for(int row = 0; row < HEIGHT; row++) {
        for(int col = 0; col < WIDTH; col++) {
            int index = row * WIDTH + col;
            if ((red >> index) & 1) {
                // check horizontal
                if (col + 3 < WIDTH && ((red >> (row * WIDTH + col)) & 1) &&
                    ((red >> (row * WIDTH + col + 1)) & 1) &&
                    ((red >> (row * WIDTH + col + 2)) & 1) &&
                    ((red >> (row * WIDTH + col + 3)) & 1)) {
                    return TerminalState::RED_WON;
                }
                // check vertical
                if (row + 3 < HEIGHT && ((red >> index) & 1) &&
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
                if (col + 3 < WIDTH && row + 3 < WIDTH && ((red >> index) & 1) &&
                    ((red >> ((row + 1) * WIDTH + col + 1)) & 1) &&
                    ((red >> ((row + 2) * WIDTH + col + 2)) & 1) &&
                    ((red >> ((row + 3) * WIDTH + col + 3)) & 1)) {
                    return TerminalState::RED_WON;
                }
            } else if((yellow >> index) & 1){
                // check horizontal
                if (col + 3 < WIDTH && ((yellow >> (row * WIDTH + col)) & 1) &&
                    ((yellow >> (row * WIDTH + col + 1)) & 1) &&
                    ((yellow >> (row * WIDTH + col + 2)) & 1) &&
                    ((yellow >> (row * WIDTH + col + 3)) & 1)) {
                    return TerminalState::YELLOW_WON;
                }
                // check vertical
                if (row + 3 < HEIGHT && ((yellow >> index) & 1) &&
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
                if (col + 3 < WIDTH && row + 3 < HEIGHT && ((yellow >> index) & 1) &&
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

int pointScore(int player, int opponent, int empty) {
    if (player == 3 && empty == 1) {
        return 100;
    } else if (player == 2 && empty == 2) {
        return 50;
    } else if (opponent == 3 && empty == 1) {
        return -80;
    } else if (opponent == 2 && empty == 2) {
        return -30;
    }
    return 0;
}

int Board::heuristic() {
    static const int dRow[] = {0, 1, -1, 1};
    static const int dCol[] = {1, 0, 1, 1};

    int score = 0;

    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            for (int d = 0; d < 4; d++) {
                int endRow = row + 3 * dRow[d];
                int endCol = col + 3 * dCol[d];

                if (endRow < 0 || endRow >= HEIGHT || endCol < 0 || endCol >= WIDTH)
                    continue;

                int yellow = 0, red = 0, empty = 0;

                for (int i = 0; i < 4; i++) {
                    int r = row + i * dRow[d];
                    int c = col + i * dCol[d];
                    Tile cell = getTile(r, c);
                    if (cell == Tile::RED) red++;
                    else if (cell == Tile::YELLOW) yellow++;
                    else if (cell == Tile::EMPTY) empty++;
                }

                if (yellow > 0 && red > 0) continue;
                score += pointScore(yellow, red, empty);
            }
        }
    }

    return score;
}

bool Board::validMove(int col) const {
    return !(((red >> col) | (yellow >> col)) & 1);
}

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