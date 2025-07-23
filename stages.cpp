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
	for (int p = 0; p < board.playerCount; p++) {
		std::cout << "Player " << board.players[p].id << " mine count: " << board.players[p].mineCount << std::endl;
	}
	printBoard(board);
	std::cout << "Press enter to commence the round!" << std::endl;
}

int gameStages::minePlacement(Board &board) {
	for (int p = 0; p < board.playerCount; p++) {
		std::cout << "Player " << board.players[p].id << "'s turn to place their mines:" << std::endl;
		chooseMinePositions(board, board.players[p]);
	}
	std::cout << "ea" << std::endl;
	bool wasThereCollision = checkMineCollision(board); // check if mines collide, if they do, notify the player
	if (wasThereCollision == true) {
		waitForInput();
	}
	std::cout << "ea" << std::endl;

	eliminatePlayers(board);
	std::cout << "ea" << std::endl;

	return gameEndCondition(board); // every end step, check if a winner has been decided
}

int gameStages::guessing(Board &board) {
	for (int p = 0; p < board.playerCount; p++) {
		std::cout << "Player " << board.players[p].id << "'s turn to guess:" << std::endl;
		guess(board, board.players[p]);
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