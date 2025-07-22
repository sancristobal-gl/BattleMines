#include "stages.h"

void waitForInput(){
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	system("pause");
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
	bool wasThereCollision = checkMineCollision(board); // check if mines collide, if they do, notify the player
	if (wasThereCollision == true) {
		std::cout << "DEBUG: JKRGNWRLIGNWRJGNWRL" << std::endl;
		waitForInput();
		std::cout << "DEBUG: JKRGNWRLIGNWRJGNWRL" << std::endl;
	}
	eliminatePlayers(board);
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

void gameStages::roundEnd(Board &board) {
	disableTilesUsed(board);   // tiles that were in play during this turn will become unavailable for the rest of the game
	board.placedMines.clear(); // clear all mines from the field
}