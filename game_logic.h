#include "board_console.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

Board createBoard();

Board createBoard(int gameType, int width, int height, int mineCount, int playerCount);

Position getPlayerInput(Board const &board, Player const &player);

void chooseMinePositions(Board &board, Player &player);

void guess(Board &board, Player &player);

bool checkMineCollision(Board &board);