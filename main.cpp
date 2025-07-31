#include "stages.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

unsigned int initRand(std::optional<unsigned int> customSeed = std::nullopt) { // initialize randomizer with either random or custom seed
	unsigned int seed = customSeed.value_or(time(0));
	srand(seed);
	return seed;
}
int gameLoop(Board &board) {
	/*
	winner = 0, tie
	winner = 1, player 1 won
	winner = 2, player 2 won
	etc
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
}
<<<<<<< HEAD
//  TODO: Fix available spaces < mine count soft-block edge case
=======
>>>>>>> 30d3d92206930a0db28b891f3757077bcec6f80c
int main() {
#ifdef LOG
	std::ofstream log("seed.txt");
	log << initRand();
	log.close();
#endif
#ifdef QUICKSIM
	Board board = createBoard(2, 10, 10, 3, 8);
#else
	Board board = createBoard();
#endif
	int winner = gameLoop(board);
	if (winner != 0) {
		std::cout << "The winner is: player " << winner << "! Congratulations!" << std::endl;
	} else {
		std::cout << "The game is a draw!" << std::endl;
	}
}
