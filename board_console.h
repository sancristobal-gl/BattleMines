#include "board.h"
#include <functional>

void setAwaitUserInput(bool value);

void getUserInput();

Board createBoard();

void printToPlayer(Player player, std::string message);

int getValuesWithinRange(std::string const &prompt, int min, int max);

void printBoard(Board const &board, int perspective = -1);