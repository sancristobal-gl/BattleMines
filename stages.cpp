#include "stages.h"

void waitForInput() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << "Press enter to continue" << std::endl;
	std::cin.get();
	system("cls");
	return;
}

void gameStages::roundStart(Board const &board) {
	for (Player const &p: board.players) {
		std::cout << "Player " << p.id << " mine count: " << p.mineCount << std::endl;
	}
	printBoard(board);
	std::cout << "Press enter to commence the round!" << std::endl;
	waitForInput();
}

int gameStages::minePlacement(Board &board) {
	for (Player &p: board.players) {
		std::cout << "Player " << p.id << "'s turn to place their mines:" << std::endl;
		chooseMinePositions(board, p);
	}
	bool wasThereCollision = checkMineCollision(board); // check if mines collide, if they do, notify the player
	if (wasThereCollision == true) {
		waitForInput();
	}
	eliminatePlayers(board);
	return gameEndCondition(board); // every end step, check if a winner has been decided
}

int gameStages::guessing(Board &board) {
	for (Player &p: board.players) {
		std::cout << "Player " << p.id << "'s turn to guess:" << std::endl;
		guess(board, p);
		waitForInput();
	}
	eliminatePlayers(board);
	return gameEndCondition(board);
}

int gameStages::roundEnd(Board &board) {
	system("cls");
	disableTilesUsed(board);		// tiles that were in play during this turn will become unavailable for the rest of the game
	board.placedMines.clear();		// clear all mines from the field
	return gameEndCondition(board); // for "not enough tiles remaining" draw condition
}