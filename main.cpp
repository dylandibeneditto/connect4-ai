#include <cstdlib>
#include <iostream>
#include <algorithm>

const int WIDTH = 7;
const int HEIGHT = 6;

// ANSI escape codes
const int BLUE = 34;
const int RED = 31;
const int YELLOW = 33;
const int GRAY = 90;
const int BGRED = 101;
const int BGYELLOW = 103;

/// @brief generic class to store any state of the board
class Board {
public:
    /** @brief current move in boolean form
     *
     * @param 0: red's turn
     * @param 1: yellow's turn
     */
    bool move = 0;

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

    std::cout << " ";
    for (int i = 1; i <= WIDTH; i++) {
        printf("%c[%dm  %d ", 0x1B, GRAY, i);
    }
    std::cout << std::endl;
}

/** @brief drops the current player's piece into the given column
 *
 * @attention assumes column is a valid move
 * 
 * @param board: current board
 * @param column: the column which the piece will be dropped down (starts at
 * zero)
 *
 * @return the new instance of the board
 */
Board makeMove(const Board& board, int column) {
    Board newBoard;
    newBoard.move = 1 - board.move;

    // Copy the side array
    for (int i = 0; i < HEIGHT; ++i) {
        std::copy(board.side[i], board.side[i] + WIDTH, newBoard.side[i]);
    }

    // Copy the mask array
    for (int i = 0; i < HEIGHT; ++i) {
        std::copy(board.mask[i], board.mask[i] + WIDTH, newBoard.mask[i]);
    }

    for (int i = HEIGHT - 1; i >= 0; i--) {
        if (!newBoard.mask[i][column]) {
            newBoard.mask[i][column] = 1;
            newBoard.side[i][column] = newBoard.move;
            break;
        }
    }
    return newBoard;
}

int main() {
    Board workingBoard;
    bool gameOver = false;
    while (!gameOver) {
        display(workingBoard);
        int move;
        printf("%c[%dm(enter your move): ", 0x1B, GRAY);
        printf("%c[%dm", 0x1B, 97);  // reset to white for input
        std::cin >> move;
        move -= 1;
        if (move >= 0 && move < WIDTH) {
            workingBoard = makeMove(workingBoard, move);
        } else {
            printf("%c[%dmInvalid move. Try again.\n", 0x1B, RED);
        }
    }
    return 0;
}