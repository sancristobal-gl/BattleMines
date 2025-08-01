#include "board.h"
#include "functional"

void waitForInput();

void setAwaitUserInput(bool value);

Board createBoard();

void printToPlayer(Player const &player, std::string const &message);

int getValuesWithinRange(std::string const &prompt, int min, int max);

void printBoard(Board const &board, int perspective = -1);