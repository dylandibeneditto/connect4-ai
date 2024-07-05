#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

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

//
//  Game Logic and Helper Functions
//

/** @brief returns whether column is a valid spot to move
 *
 * @param board: current board
 * @param column: the column which needs to get checked
 */
bool isValidColumn(const Board& board, int column) {
  return board.mask[0][column] == 0;
}

/** @brief returns whether the given position is drawn
 *
 * @param board: current board
 */
bool isDrawn(const Board& board) {
  for (int i = 0; i < WIDTH; i++) {
    if (isValidColumn(board, i)) {
      return false;
    }
  }
  return true;
}

int largestLine(const Board& board, bool move, int i, int j) {
  int result = 0;
  // horizontal check (-)
  int count = 0;
  for (int x = std::max(j - 3, 0); x <= std::min(j + 3, WIDTH - 1); x++) {
    if (board.mask[i][x] == 1 && board.side[i][x] == move) {
      count++;
    } else {
      count = 0;
    }
  }

  result = std::max(result, count);

  // vertical check (|)
  count = 0;
  for (int y = std::max(i - 3, 0); y <= std::min(i + 3, HEIGHT - 1); y++) {
    if (board.mask[y][j] == 1 && board.side[y][j] == move) {
      count++;
    } else {
      count = 0;
    }
  }

  result = std::max(result, count);

  // diagonal check (\)
  count = 0;
  for (int x = -3; x <= 3; x++) {
    int y = i + x;
    int k = j + x;
    if (y >= 0 && y < HEIGHT && k >= 0 && k < WIDTH) {
      if (board.mask[y][k] == 1 && board.side[y][k] == move) {
        count++;
      } else {
        count = 0;
      }
    }
  }

  result = std::max(result, count);

  // diagonal check (/)
  count = 0;
  for (int x = -3; x <= 3; x++) {
    int y = i + x;
    int k = j - x;
    if (y >= 0 && y < HEIGHT && k >= 0 && k < WIDTH) {
      if (board.mask[y][k] == 1 && board.side[y][k] == move) {
        count++;
      } else {
        count = 0;
      }
    }
  }

  result = std::max(result, count);

  return result;
}

/** @brief checks any position on the board to see if it is within a four in a
 * row
 *
 * @param board: current board
 * @param move: the players move to be checked
 * @param i: the y position to be checked (0-ed)
 * @param j: the x position to be checked (0-ed)
 *
 */
bool isFourInRow(const Board& board, bool move, int i, int j) {
  if (largestLine(board, move, i, j) >= 4) {
    return true;
  } else {
    return false;
  }
}

/** @brief sees if a position is won from a given players pieces
 *
 * @param board: current board
 * @param move: players move to be checked
 */
bool isWon(const Board& board, bool move) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (board.side[i][j] == move) {
        if (isFourInRow(board, move, i, j)) {
          return true;
        }
      }
    }
  }
  return false;
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
  for (int i = 0; i < HEIGHT; i++) {
    std::copy(board.side[i], board.side[i] + WIDTH, newBoard.side[i]);
  }

  // Copy the mask array
  for (int i = 0; i < HEIGHT; i++) {
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

//
//  AI Functions
//

int heuristic(const Board& board, bool move) {
  int result = 0;
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (board.side[i][j] == move) {
        result += largestLine(board, move, i, j);
      }
    }
  }
  return result;
}

int minimax(Board board, int depth, bool maximizingPlayer, int alpha,
            int beta) {
  if (isWon(board, maximizingPlayer)) {
    return -10000;
  } else if (isDrawn(board)) {
    return 0;
  }
  if (depth == 0) {
    return heuristic(board, maximizingPlayer);
  }

  if (maximizingPlayer) {
    int maxEval = -10000;
    for (int col = 0; col < WIDTH; col++) {
      if (isValidColumn(board, col)) {
        Board newBoard = makeMove(board, col);
        int eval = minimax(newBoard, depth - 1, false, alpha, beta);
        maxEval = std::max(maxEval, eval);
        alpha = std::max(alpha, eval);
        if (beta <= alpha) {
          break;
        }
      }
    }
    return maxEval;
  } else {
    int minEval = 10000;
    for (int col = 0; col < WIDTH; col++) {
      if (isValidColumn(board, col)) {
        Board newBoard = makeMove(board, col);
        int eval = minimax(newBoard, depth - 1, true, alpha, beta);
        minEval = std::min(minEval, eval);
        beta = std::min(beta, eval);
        if (beta <= alpha) {
          break;
        }
      }
    }
    return minEval;
  }
}

int findBestMove(Board board, int depth) {
  int bestMove = -1;
  int bestValue = -10000;
  for (int col = 0; col < WIDTH; col++) {
    if (isValidColumn(board, col)) {
      Board newBoard = makeMove(board, col);
      int boardValue = minimax(newBoard, depth - 1, false, -10000, 10000);
      if (boardValue > bestValue) {
        bestValue = boardValue;
        bestMove = col;
      }
    }
  }
  return bestMove;
}

//
//  Display Functions
//

/// @brief clears the screen based on operating system
void clear() {
#ifdef WINDOWS
  std::system("cls");
#else
  std::system("clear");
#endif
}

/// @brief gets the input for the next move
/// @param board: current board
/// @return 0-ed input provided by the user
int getMove(const Board& board) {
  int move;
  std::cin >> move;
  move -= 1;
  if (move >= 0 && move < WIDTH && isValidColumn(board, move)) {
    return move;
  } else {
    printf("%c[%dmInvalid move. Try again.\n", 0x1B, RED);
    return -1;
  }
}

/// @brief displays the board after clearing the screen
/// @param board: current board
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

int main() {
  Board workingBoard;
  bool gameOver = false;
  bool aiOnly = false;
  bool userMove = 0;

  if (!aiOnly && userMove == 1) {  // when AI makes the first move
    workingBoard = makeMove(workingBoard, rand() % WIDTH);
  }

  while (!gameOver) {
    display(workingBoard);
    if (isDrawn(workingBoard)) {
      gameOver = true;
      printf("%c[%dmGAME OVER: DRAW", 0x1B, 97);
      break;
    } else if (isWon(workingBoard, 0)) {
      gameOver = true;
      printf("%c[%dmGAME OVER: RED WON", 0x1B, 97);
      break;
    } else if (isWon(workingBoard, 1)) {
      gameOver = true;
      printf("%c[%dmGAME OVER: YELLOW WON", 0x1B, 97);
      break;
    }

    if (aiOnly) {
      workingBoard = makeMove(workingBoard, findBestMove(workingBoard, 5));
    } else {
      int move;
      printf("%c[%dm(enter your move): ", 0x1B, GRAY);
      printf("%c[%dm", 0x1B, 97);  // reset to white for input
      move = getMove(workingBoard);
      if (move > -1) {
        workingBoard = makeMove(workingBoard, move);
        if (!isWon(workingBoard, 0) && !isDrawn(workingBoard)) {
          workingBoard = makeMove(workingBoard, findBestMove(workingBoard, 10));
        }
      }
    }
  }

  return 0;
}