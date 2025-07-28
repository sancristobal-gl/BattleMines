#include "stages.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <fstream> //for logging to file

int initRand() { // get random seed
	int seed = time(0);
	srand(seed);
	return seed;
}

int initRand(int seed) { // set custom seed for debugging purpouses
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

// TODO: Fix available spaces < mine count soft-block edge case
int main() {
	std::ofstream log("seed.txt");
	log << initRand();
	log.close();
	Board board = createBoard(2, 10, 10, 8, 8);
	//Board board = createBoard();
	int winner = gameLoop(board);
	if (winner != 0) {
		std::cout << "The winner is: player " << winner << "! Congratulations!" << std::endl;
	} else {
		std::cout << "The game is a draw!" << std::endl;
	}
}