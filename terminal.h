#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>

void enableRawMode();
void disableRawMode();
std::string getKeyInput();
void clearScreen();
void moveCursorToTop();

#endif