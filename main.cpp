#include <cstdlib>  // Include for std::system
#include <iostream>

const int WIDTH = 7;
const int HEIGHT = 6;

// ANSI escape codes
const int BLUE = 34;
const int RED = 31;
const int YELLOW = 33;
const int GRAY = 90;

/// @brief generic class to store any state of the board
class Board {
public:
    /** @brief board array which uses binary to display the side of a tile
     *
     * @attention this array relies on the `mask` array to determine whether a 0
     * is red or an empty tile
     *
     * @param 0: red
     * @param 1: yellow
     */
    bool side[HEIGHT][WIDTH];
    /** @brief board array which uses binary to display the empty state of a tile
     *
     * @param 0: empty
     * @param 1: filled
     */
    bool mask[HEIGHT][WIDTH];

    Board() {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                side[i][j] = 0;
                mask[i][j] = 0;
            }
        }
    }
};

/// @brief clears the screen based on operating system
void clear() {
#ifdef WINDOWS
    std::system("cls");
#else
    std::system("clear");
#endif
}

/// @brief displays the board after clearing the screen
/// @param board
void display(const Board& board) {
    clear();
    for (int i = 0; i < HEIGHT; i++) {
        printf("%c[%dm | ", 0x1B, BLUE);
        for (int j = 0; j < WIDTH; j++) {
            if (board.mask[i][j] == 1) {
                if (board.side[i][j] == 0) {
                    printf("%c[%dmO", 0x1B, RED);
                } else {
                    printf("%c[%dmO", 0x1B, YELLOW);
                }
            } else {
                std::cout << " ";
            }
            printf("%c[%dm | ", 0x1B, BLUE);
        }
        std::cout << std::endl;
    }

    std::cout<<" ";
    for (int i = 1; i <= WIDTH; i++) {
        printf("%c[%dm  %d ", 0x1B, GRAY, i);
    }
    std::cout << std::endl;
}

int main() {
    Board workingBoard;
    display(workingBoard);
    return 0;
}