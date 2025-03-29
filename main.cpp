#include <iostream>
#include "terminal.h"
#include "board.h"

void printGameBoard(Board board, int position, bool player) {
    for (int row = 0; row < 7; row++) {
        for (int col = 0; col < 7; col++) {
            if (row == 0) {
                if (col == position)
                    printf("  %c[%dmO ", 0x1B, player ? 93 : 91);
                else
                    std::cout << "    ";
                continue;
            }

            Board::Tile tile = board.getTile(row-1, col);
            printf("%c[%dm|", 0x1B, 34);
            if (tile == Board::Tile::EMPTY) {
                if (col == position && (row == 6 || board.getTile(std::min(6,row), col) != Board::Tile::EMPTY))
                    printf(" %c[%dmO ", 0x1B, player ? 33 : 31);
                else
                    std::cout << "   ";
            } else if (tile == Board::Tile::RED) {
                printf("%c[%dm O ", 0x1B, 91);
            } else if (tile == Board::Tile::YELLOW) {
                printf("%c[%dm O ", 0x1B, 93);
            }
        }
        printf("%c[%dm%s\n", 0x1B, 34, row == 0 ? "" : "|");

    }
    printf("%c[%dm", 0x1B, 39);
}

void getFourInRow(Board board, int tiles[4][2], Board::TerminalState state) {
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            Board::Tile tile = board.getTile(row, col);
            if ((state == Board::TerminalState::RED_WON && tile == Board::Tile::RED) ||
                (state == Board::TerminalState::YELLOW_WON && tile == Board::Tile::YELLOW)) {
                
                // Check horizontal
                if (col + 3 < 7 &&
                    board.getTile(row, col + 1) == tile &&
                    board.getTile(row, col + 2) == tile &&
                    board.getTile(row, col + 3) == tile) {
                    tiles[0][0] = row; tiles[0][1] = col;
                    tiles[1][0] = row; tiles[1][1] = col + 1;
                    tiles[2][0] = row; tiles[2][1] = col + 2;
                    tiles[3][0] = row; tiles[3][1] = col + 3;
                    return;
                }

                // Check vertical
                if (row + 3 < 6 &&
                    board.getTile(row + 1, col) == tile &&
                    board.getTile(row + 2, col) == tile &&
                    board.getTile(row + 3, col) == tile) {
                    tiles[0][0] = row; tiles[0][1] = col;
                    tiles[1][0] = row + 1; tiles[1][1] = col;
                    tiles[2][0] = row + 2; tiles[2][1] = col;
                    tiles[3][0] = row + 3; tiles[3][1] = col;
                    return;
                }

                // Check diagonal (bottom-left to top-right)
                if (row - 3 >= 0 && col + 3 < 7 &&
                    board.getTile(row - 1, col + 1) == tile &&
                    board.getTile(row - 2, col + 2) == tile &&
                    board.getTile(row - 3, col + 3) == tile) {
                    tiles[0][0] = row; tiles[0][1] = col;
                    tiles[1][0] = row - 1; tiles[1][1] = col + 1;
                    tiles[2][0] = row - 2; tiles[2][1] = col + 2;
                    tiles[3][0] = row - 3; tiles[3][1] = col + 3;
                    return;
                }

                // Check diagonal (top-left to bottom-right)
                if (row + 3 < 6 && col + 3 < 7 &&
                    board.getTile(row + 1, col + 1) == tile &&
                    board.getTile(row + 2, col + 2) == tile &&
                    board.getTile(row + 3, col + 3) == tile) {
                    tiles[0][0] = row; tiles[0][1] = col;
                    tiles[1][0] = row + 1; tiles[1][1] = col + 1;
                    tiles[2][0] = row + 2; tiles[2][1] = col + 2;
                    tiles[3][0] = row + 3; tiles[3][1] = col + 3;
                    return;
                }
            }
        }
    }
}

void printFinalBoard(Board board, Board::TerminalState state) {

    int tiles[4][2] = {{0,0}, {0,0}, {0,0}, {0,0}};

    getFourInRow(board, tiles, state);

    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            int background = (state == Board::TerminalState::DRAW) ? 100 : 40;

            for(auto& i : tiles) {
                if(row == i[0] && col == i[1]) {
                    background = 100;
                    break;
                }
            }

            Board::Tile tile = board.getTile(row, col);

            printf("\x1b[%dm|", 34);

            if (tile == Board::Tile::EMPTY) {
                printf("   ");
            } else if (tile == Board::Tile::RED) {
                printf("\x1b[%d;%dm O ", 31, background);
            } else if (tile == Board::Tile::YELLOW) {
                printf("\x1b[%d;%dm O ", 33, background);
            }

            printf("\x1b[0m");
        }

        printf("\x1b[34m|\n");
        printf("\x1b[0m");
    }

    printf("\x1b[0m");
}

void printBoard(Board board) {
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            Board::Tile tile = board.getTile(row, col);
            printf("%c[%dm|", 0x1B, 34);
            if (tile == Board::Tile::EMPTY) {
                std::cout << "   ";
            } else if (tile == Board::Tile::RED) {
                printf("%c[%dm O ", 0x1B, 31);
            } else if (tile == Board::Tile::YELLOW) {
                printf("%c[%dm O ", 0x1B, 33);
            }
        }
        printf("%c[%dm|\n", 0x1B, 34);

    }
    printf("%c[%dm", 0x1B, 39);
}

int minimax(Board board, int depth, int alpha, int beta, bool maximizing) {
    Board::TerminalState state = board.terminal();
    if (state == Board::TerminalState::RED_WON) {
        return -99000 - depth;
    } else if (state == Board::TerminalState::YELLOW_WON) {
        return 99000 + depth;
    } else if (state == Board::TerminalState::DRAW) {
        return 0;
    }

    if (depth == 0) {
        return board.heuristic();
    }

    if (maximizing) {
        int maxEval = -100000;
        for (int col = 0; col < 7; col++) {
            if (board.validMove(col)) {
                Board boardCopy = Board(board);
                boardCopy.dropTile(col);
                int eval = minimax(boardCopy, depth - 1, alpha, beta, false);
                maxEval = std::max(maxEval, eval);
                if(maxEval > beta)
                    break;
                alpha = std::max(alpha, maxEval);
            }
        }
        return maxEval;
    } else {
        int minEval = 100000;
        for (int col = 0; col < 7; col++) {
            if (board.validMove(col)) {
                Board boardCopy = Board(board);
                boardCopy.dropTile(col);
                int eval = minimax(boardCopy, depth - 1, alpha, beta, true);
                minEval = std::min(minEval, eval);
                if(minEval < alpha)
                    break;
                beta = std::min(beta, minEval);
            }
        }
        return minEval;
    }
}

int findBestMove(Board board, int depth, bool player) {
    if(!player) {
        int bestMove = -1;
        int bestValue = -100000;

        for (int col = 0; col < 7; col++) {
            if (board.validMove(col)) {
                Board boardCopy = Board(board);
                boardCopy.dropTile(col);
                int moveValue = minimax(boardCopy, depth - 1, -1000000, 1000000, player);
                if (moveValue > bestValue) {
                    bestValue = moveValue;
                    bestMove = col;
                }
            }
        }
        return bestMove;
    } else {
        int bestMove = -1;
        int bestValue = 100000;

        for (int col = 0; col < 7; col++) {
            if (board.validMove(col)) {
                Board boardCopy = Board(board);
                boardCopy.dropTile(col);
                int moveValue = minimax(boardCopy, depth - 1, -1000000, 1000000, player);
                std::cout << moveValue << "\n\b";
                if (moveValue < bestValue) {
                    bestValue = moveValue;
                    bestMove = col;
                }
            }
        }
        return bestMove;
    }
}

int main() {
    enableRawMode();
    atexit(disableRawMode);

    int position = 3;
    Board board;

    bool running = true;
    bool player = 0;

    bool pregame = true;

    while(pregame) {
        moveCursorToTop();
        clearScreen();
        std::cout << "Welcome to Connect 4\n";
        std::cout << "Use arrow keys to move, press space to select, or 'q' to quit.\n";
        printf("%s%c[%dmRED\t\t%c[%dm(first move)\n", (!player ? " > " : "   "), 0x1B, 31, 0x1B, 30);
        printf("%c[%dm", 0x1B, 39);
        printf("%s%c[%dmYELLOW\t%c[%dm(second move)\n", (player ? " > " : "   "), 0x1B, 33, 0x1B, 30);
        printf("%c[%dm", 0x1B, 39);
        std::string key = getKeyInput();
        if (key == " ") {
            pregame = false;
        } else if (key == "UP") {
            player = 0;
        } else if (key == "DOWN") {
            player = 1;
        } else if (key == "q") {
            return 0;
        }
    }

    if(player) {
        board.dropTile(3); // 3 has been proven to be the best move
    }

    while (running) {

        moveCursorToTop();
        clearScreen();

        printGameBoard(board, position, player);
        std::cout << "Use arrow keys to move, space to drop, and 'q' to quit.\n";


        std::string key = getKeyInput();
        if (key == "q") {
            running = false;
        } else if (key == "LEFT") {
            if (position > 0) position--;
        } else if (key == "RIGHT") {
            if (position < 6) position++;
        } else if (key == " ") {
            if(board.validMove(position)) {
                board.dropTile(position);
                board.dropTile(findBestMove(board, 9, player));
            }
        }


        Board::TerminalState state = board.terminal();
        if(state == Board::TerminalState::IN_PROGRESS) {
            continue;
        }
        clearScreen();
        printFinalBoard(board, state);
        if(state == Board::TerminalState::DRAW) {
            std::cout << "It's a draw\n";
            break;
        } else if(state == Board::TerminalState::RED_WON) {
            std::cout << "Red wins\n";
            break;
        } else if(state == Board::TerminalState::YELLOW_WON) {
            std::cout << "Yellow wins\n";
            break;
        }
    }

    return 0;
}