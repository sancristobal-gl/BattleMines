#include "board_console_implementation.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

std::string getPlayerInput(Board &board, Player player, int askCoordinate);

void chooseMinePositions(Board &board, Player &player);

void guess(Board &board, Player &player);

bool chechMineCollision(Board &board);