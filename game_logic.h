#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime>
#include <vector>
#include <unordered_map>
#include <utility>
#include "board.h"

std::string getPlayerInput(Board &board, Player player, int askCoordinate);

void chooseMinePositions(Board &board, Player &player);

void guess(Board &board, Player & player);

bool chechMineCollision(Board &board);