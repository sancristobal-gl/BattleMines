#include "game_logic.h"

int roundStartStage(Board board);

int minePlacementStage(Board board, int winner);

int guessingStage(Board board, int winner);

void roundEndStage(Board board);

int gameLoop(Board &board);