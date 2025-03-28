#include <iostream>
#include "terminal.h"
#include "board.h"

int main() {
    enableRawMode();
    atexit(disableRawMode);

    int position = 3;
    Board board;

    bool running = true;
    while (running) {
        moveCursorToTop();
        clearScreen();

        for (int i = 0; i < 7; ++i) {
            if (i == position)
                std::cout << "  v  ";
            else
                std::cout << "     ";
        }
        std::cout << "\n";

        for (int row = 0; row < 6; row++) {
            for (int col = 0; col < 7; col++) {
                Board::Tile tile = board.getTile(row, col);
                if (tile == Board::Tile::EMPTY) {
                    std::cout << "|   |";
                } else if (tile == Board::Tile::RED) {
                    std::cout << "| R |";
                } else if (tile == Board::Tile::YELLOW) {
                    std::cout << "| Y |";
                }
            }
            std::cout << "\n";
        }

        std::string key = getKeyInput();
        if (key == "q") {
            running = false;
        } else if (key == "LEFT") {
            if (position > 0) position--;
        } else if (key == "RIGHT") {
            if (position < 6) position++;
        } else if (key == " ") {
            std::cout << "Dropping in column " << position << "\n";
            board.dropTile(position);
        }
    }

    return 0;
}