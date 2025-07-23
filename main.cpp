#include "stages.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

int gameLoop(Board &board) {
	/*
	winner = 0, empate
	winner = 1, gano jugador 1
	winner = 2, gano jugador 2
	*/
	int winner = -1;
	while (winner == -1) { // could probably iterate over an array of function pointers?
		gameStages::roundStart(board);
		if (winner != -1) return winner; // not needed here but it looks more symmetrical
		winner = gameStages::minePlacement(board);
		if (winner != -1) return winner;
		winner = gameStages::guessing(board);
		if (winner != -1) return winner;
		winner = gameStages::roundEnd(board);
		if (winner != -1) return winner;
	}
	return winner;
	return 0;
}

// TODO: Fix available spaces < mine count soft-block edge case
int main() {
	srand(time(0)); // initialize random number generator
	Board board = createBoard();
	int winner = gameLoop(board);
	if (winner != 0) {
		std::cout << "The winner is: player " << winner << "! Congratulations!" << std::endl;
	} else {
		std::cout << "The game is a draw!" << std::endl;
	}
}