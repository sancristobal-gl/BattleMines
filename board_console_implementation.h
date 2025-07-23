#include "board.h"

Board createBoard();

int getValuesWithinRange(std::string const &prompt, int min, int max);

void printBoard(Board const &board, int perspective = -1);