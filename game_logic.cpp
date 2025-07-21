#include "game_logic.h"

// gets input from console if player is human
// generates random input if player is bot
Board createBoard() {
	Board board;
	bool created = false;
	int width = NULL;
	int height = NULL;
	int mineCount = 0;
	board.gameType = static_cast<gameType>(getValuesWithinRange("choose game mode (0=PVP, 1=PVE)", 0, 1)); // not sure if this is good practice
	board.width = getValuesWithinRange("choose the width of the field", 5, 10);
	board.height = getValuesWithinRange("choose the height of the field", 5, 10);
	mineCount = getValuesWithinRange("choose the number of mines on the field", 3, 8);
	board.playerCount = getValuesWithinRange("choose the number of players", 2, 8);
	board.players = new Player[board.playerCount]; // dynamic array since we don't know the number of players beforehand
	for (int p = 0; p < board.playerCount; p++) {
		Player player;
		player.mineCount = mineCount;
		player.id = p + 1;
		if (board.gameType == PVE) {
			if (p > 0) {
				player.isAI = true;
			}
		}
		board.players[p] = player;
	}
	return board;
}

Position getPlayerInput(Board &board, Player player) {
	Position pos;
	if (player.isAI == false) {
		std::cout << "x: ";
		std::cin >> pos.xpos;
		std::cout << "y: ";
		std::cin >> pos.ypos;
	} else {
		std::vector<Position> validTiles = getValidTiles(board); // TODO: make bot not be able to choose the positions where their own mines are placed
		int randomPos = rand() % validTiles.size();
		pos = validTiles[randomPos];
		std::cout << std::endl;
	}
	return pos;
}

void chooseMinePositions(Board &board, Player &player) {
	std::cout << "Player " << player.id << "!, choose your mine's positions" << std::endl;
	for (int m = 0; m < player.mineCount; m++) {
		printBoard(board, player.id);
		bool validPlacement = false;
		Mine mine;
		while (validPlacement == false) {
			std::cout << "Choose the position of mine " << m << std::endl;
			mine.position = getPlayerInput(board, player);
			mine.owner = player.id;
			validPlacement = isPositionValid(board, mine.position);
			if (validPlacement == false) {
				std::cout << "Invalid position! Choose again" << std::endl;
			}
			for (std::vector<Mine>::iterator it = board.placedMines.begin(); it != board.placedMines.end(); it++) {
				if (*it == mine) { // for them to be equal, they need to share position and owner
					std::cout << "You already placed a mine there! Choose again" << std::endl;
					validPlacement = false;
				}
			}
		}
		// std::cout << "Player " << mine.owner+1 <<  " placed mine at " << mine.xpos << ", " << mine.ypos << std::endl; //for testing purposes, TODO: remove before main release
		board.placedMines.push_back(mine);
		std::cout << std::endl;
		system("cls");
	}
}

void guess(Board &board, Player &player) {
	printBoard(board, player.id);
	Position guess;
	int isGuessValid = false; // flag to check if the inputed position is valid. If not, ask the player again
	while (isGuessValid == false) {
		std::cout << "Player " << player.id << ", take a guess... " << std::endl;
		guess = getPlayerInput(board, player);
		isGuessValid = isPositionValid(board, guess);
		if (isGuessValid == false) {
			std::cout << "That spot has already been checked! Try again" << std::endl;
		}
		// system("cls");
	}
	// check if the guessed position shares the same position as a mine
	std::vector<Mine> minesToRemove;
	for (std::vector<Mine>::iterator it = board.placedMines.begin(); it != board.placedMines.end(); it++) {
		if (*it == guess) {
			if (it->owner == player.id) {
				std::cout << "That's your own mine, silly!" << std::endl; // el jugador pierde la oportunidad de volver a adivinar
			} else {
				removeMine(board, *it);
				disablePosition(board, guess);
				std::cout << "You found an enemy mine!" << std::endl;
				break;
			}
		} else
			disablePosition(board, guess);
	}
	std::cout << "No mine has been found" << std::endl;
	return;
}

// function to be called after each player has placed their mines
// colliding mines are annihilated and removed from their owner's mine pool
bool chechMineCollision(Board &board) {
	bool wasThereCollision = false;
	std::vector<Mine> conflictingMines;
	for (int i = 0; i < board.placedMines.size() - 1; i++) { //size is -- because the vector value in i is compared to the other values on his right
		std::cout << "DEBUG clearing mines... iteration: " << i << " placedMines size: " << board.placedMines.size() << std::endl;
		Position mine1Pos = board.placedMines[i].position;
		conflictingMines.push_back(board.placedMines[i]);
		for (int j = i + 1; j < board.placedMines.size(); j++) {
			Position mine2Pos = board.placedMines[j].position;
			if (mine1Pos.xpos == mine2Pos.xpos && mine1Pos.ypos == mine2Pos.ypos) { // they'll be equal if they share the same positoon, the owner is not a factor
				conflictingMines.push_back(board.placedMines[j]);
			}
		}
		if (conflictingMines.size() > 1) {
			i--; // i is -- since the current element is to be deleted, meaning the "next" iteration should check the element that will take i's place 
			std::cout << "Colisionaron " << conflictingMines.size() << " minas en " << conflictingMines[0].position.xpos << ", " << conflictingMines[0].position.ypos << std::endl; // conflictingMines will always have a value at [0]
			for (Mine mine: conflictingMines) {
				std::cout << removeMine(board, mine) << std::endl;
				wasThereCollision = true;
			}
		}
		conflictingMines.clear();
	}
	return wasThereCollision;
}