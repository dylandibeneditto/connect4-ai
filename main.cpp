// main.cpp
#include <iostream>
#include "terminal.h"

int main() {
    enableRawMode();
    atexit(disableRawMode);

    int position = 3;

    std::cout << "Use arrow keys to move left/right. Press space to 'drop'. Press q to quit.\n";

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

        // Draw column numbers
        for (int i = 0; i < 7; ++i)
            std::cout << "  " << i << "  ";
        std::cout << "\n";

        // Dummy board
        for (int row = 0; row < 6; ++row) {
            for (int col = 0; col < 7; ++col) {
                std::cout << "|   |";
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

        }
    }

    return 0;
}