#include <iostream>
#include "terminal.h"
#include "board.h"

void printBoard(Board board) {
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            Board::Tile tile = board.getTile(row, col);
            if (tile == Board::Tile::EMPTY) {
                std::cout << "|   ";
            } else if (tile == Board::Tile::RED) {
                std::cout << "| R ";
            } else if (tile == Board::Tile::YELLOW) {
                std::cout << "| Y ";
            }
        }
        std::cout << "|\n";
    }
}

int main() {
    enableRawMode();
    atexit(disableRawMode);

    int position = 3;
    Board board;

    bool running = true;
    bool player = false;

    while (running) {

        moveCursorToTop();
        clearScreen();

        for (int i = 0; i < 7; i++) {
            if (i == position)
                std::cout << "  v ";
            else
                std::cout << "    ";
        }
        std::cout << "\n";

        printBoard(board);
        std::cout << "Use arrow keys to move, space to drop, and 'q' to quit.\n";

        std::string key = getKeyInput();
        if (key == "q") {
            running = false;
        } else if (key == "LEFT") {
            if (position > 0) position--;
        } else if (key == "RIGHT") {
            if (position < 6) position++;
        } else if (key == " ") {
            board.dropTile(position);
        }

        Board::TerminalState state = board.terminal();
        if(state == Board::TerminalState::IN_PROGRESS) {
            continue;
        }
        clearScreen();
        printBoard(board);
        if(state == Board::TerminalState::DRAW) {
            std::cout << "It's a draw!\n";
            break;
        } else if(state == Board::TerminalState::RED_WON) {
            std::cout << "Red wins!\n";
            break;
        } else if(state == Board::TerminalState::YELLOW_WON) {
            std::cout << "Yellow wins!\n";
            break;
        }
    }

    return 0;
}