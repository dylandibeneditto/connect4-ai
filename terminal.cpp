// terminal.cpp
#include "terminal.h"
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>

static struct termios orig_termios;

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

char readKey() {
    char c;
    read(STDIN_FILENO, &c, 1);
    return c;
}

std::string getKeyInput() {
    char c = readKey();
    if (c == '\x1b') {
        char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) == 0) return "ESC";
        if (read(STDIN_FILENO, &seq[1], 1) == 0) return "ESC";

        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return "UP";
                case 'B': return "DOWN";
                case 'C': return "RIGHT";
                case 'D': return "LEFT";
            }
        }
        return "ESC";
    } else {
        return std::string(1, c);
    }
}

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

void moveCursorToTop() {
    std::cout << "\033[H";
}