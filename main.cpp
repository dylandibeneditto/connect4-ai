#include <iostream>
#include "terminal.h"
#include "board.h"

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
                std::cout << moveValue << "\n\b";
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
        std::cout << (!player ? " > " : "   ") << "RED (first move)\n";
        std::cout << (player ? " > " : "   ") << "YELLOW (second move)\n";
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

        for (int i = 0; i < 7; i++) {
            if (i == position)
                printf("  %c[%dmO ", 0x1B, player ? 33 : 31);
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
        printBoard(board);
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