#include "board_console.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <optional>

Board createBoard();

Board createBoard(int gameType, int width, int height, int mineCount, int playerCount);

typedef int (*RNGPointer)(int, int);

int getRandomValueInRange(int max, int);

Position getPlayerInput(Board const &board, Player const &player, RNGPointer RNG);

void chooseMinePositions(Board &board, Player &player, RNGPointer RNG);

void guess(Board &board, Player &player, RNGPointer RNG);

bool checkMineCollision(Board &board);