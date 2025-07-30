#include "board.h"
#include <functional>

void awaitUserInput(gameType gameType);

Board createBoard();

Position getPlayerInput(Board const &board, Player player);

void printToPlayer(Player player, std::string message);

int getValuesWithinRange(std::string const &prompt, int min, int max);

void printBoard(Board const &board, int perspective = -1);