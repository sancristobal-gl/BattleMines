#include "game_logic.h"

// gets input from console if player is human
// generates random input if player is bot
Board createBoard() {
	Board board;
	bool created = false;
	int width = 0;
	int height = 0;
	int mineCount = 0;
	board.gameType = static_cast<gameType>(getValuesWithinRange("choose game mode (0=PVP, 1=PVE, 2=EVE)", 0, 2)); // not sure if this is good practice
	board.width = getValuesWithinRange("choose the width of the field", 5, 10);
	board.height = getValuesWithinRange("choose the height of the field", 5, 10);
	mineCount = getValuesWithinRange("choose the number of mines on the field", 3, 8);
	board.playerCount = getValuesWithinRange("choose the number of players", 2, 8);
	for (int p = 0; p < board.playerCount; p++) {
		Player player;
		player.mineCount = mineCount;
		player.id = p + 1;
		if (board.gameType == PVE) { // if game is PVE and the player is not the first one, they're a bot (TODO: make the user able to choose which players will be human and which will be bot)
			if (p > 0) {
				player.isAI = true;
			}
		} else if (board.gameType == EVE) { // in EVE, all players are bots
			player.isAI = true;
		}
		board.players.push_back(player);
	}
	if (board.gameType == EVE) {
		setAwaitUserInput(false);
	}
	return board;
}

Board createBoard(int gameTypeValue, int width, int height, int mineCount, int playerCount) { // for debugging purpouses
	Board board;
	board.gameType = static_cast<gameType>(gameTypeValue);
	board.width = width;
	board.height = height;
	board.playerCount = playerCount;
	for (int p = 0; p < board.playerCount; p++) {
		Player player;
		player.mineCount = mineCount;
		player.id = p + 1;
		if (board.gameType == PVE) { // if game is PVE and the player is not the first one, they're a bot (TODO: make the user able to choose which players will be human and which will be bot)
			if (p > 0) {
				player.isAI = true;
			}
		} else if (board.gameType == EVE) { // in EVE, all players are bots
			player.isAI = true;
		}
		board.players.push_back(player);
	}
	if (board.gameType == EVE) {
		setAwaitUserInput(false);
	}
	return board;
}

int getRandomValueInRange(int max, int min = 0) {
	return (rand() % (max - min) + min);
}

Position getRandomValidPosition(Board const &board, Player player) { // helper function for bot players
	// player parameter is currently unused, but it will be used in the future
	std::vector<Position> validTiles = getValidTiles(board); // TODO: make bot not be able to choose the positions where their own mines are placed
	return validTiles[getRandomValueInRange(validTiles.size())];
}
Position getPlayerInput(Board const &board, Player player) {
	Position pos;
	if (player.isAI == false) {
		std::cout << "x: ";
		std::cin >> pos.xpos;
		std::cout << "y: ";
		std::cin >> pos.ypos;
	} else {
		pos = getRandomValidPosition(board, player);
		std::cout << std::endl;
	}
	return pos;
}

void chooseMinePositions(Board &board, Player &player) {
	printToPlayer(player, std::string("Player ") + std::to_string(player.id) + std::string("!, choose your mine's positions"));
	for (int mineId = 0; mineId < player.mineCount; mineId++) {
		if (!player.isAI) {
			printBoard(board, player.id);
		}
		bool validPlacement = false;
		Mine mine;
		while (validPlacement == false) {
			printToPlayer(player, std::string("Choose the position of mine ") + std::to_string(mineId));
			mine.position = getPlayerInput(board, player);
			mine.owner = player.id;
			validPlacement = isPositionValid(board, mine.position);
			if (validPlacement == false) {
				printToPlayer(player, "Invalid position! Choose again");
			}
			for (Mine const &placedMine: board.placedMines) {
				if (placedMine == mine) { // for them to be equal, they need to share position and owner
					printToPlayer(player, "You already placed a mine there! Choose again");
					validPlacement = false;
				}
			}
		}
		// std::cout << "Player " << mine.owner+1 <<  " placed mine at " << mine.xpos << ", " << mine.ypos << std::endl; //for testing purposes, TODO: remove before main release
		board.placedMines.push_back(mine);
	}
	waitForInput();
}

void guess(Board &board, Player &player) {
	printBoard(board, player.id);
	Position guess;
	int isGuessValid = false; // flag to check if the inputed position is valid. If not, ask the player again
	while (isGuessValid == false) {
		printToPlayer(player, (std::string("Player ") + std::to_string(player.id) + std::string(", take a guess... ")));
		guess = getPlayerInput(board, player);
		isGuessValid = isPositionValid(board, guess);
		if (isGuessValid == false) {
			printToPlayer(player, "That spot has already been checked! Try again");
		}
	}
	// check if the guessed position shares the same position as a mine
	std::vector<Mine> minesToRemove;
	for (Mine const &mine: board.placedMines) {
		if (mine == guess) {
			if (mine.owner == player.id) {
				printToPlayer(player, "That's your own mine, silly!"); // el jugador pierde la oportunidad de volver a adivinar
			} else {
				removeMine(board, mine);
				printToPlayer(player, "You found an enemy mine!");
				break;
			}
		}
	}
	disablePosition(board, guess);
}

// function to be called after each player has placed their mines
// colliding mines are annihilated and removed from their owner's mine pool
bool checkMineCollision(Board &board) {
	bool wasThereCollision = false;
	std::vector<Mine> conflictingMines;
	for (int i = 0; i < board.placedMines.size() - 1; i++) { // size is -- because the vector value in i is compared to the values on its right
		if (board.placedMines.size() <= 0) {
			return wasThereCollision;
		}
		Position mine1Pos = board.placedMines[i].position;
		conflictingMines.push_back(board.placedMines[i]);
		for (int j = i + 1; j < board.placedMines.size(); j++) {
			Position mine2Pos = board.placedMines[j].position;
			if (mine1Pos.xpos == mine2Pos.xpos && mine1Pos.ypos == mine2Pos.ypos) { // they'll be equal if they share the same positoon, the owner is not a factor
				conflictingMines.push_back(board.placedMines[j]);
			}
		}
		if (conflictingMines.size() > 1) {
			i--;																																																		   // i is -- since the current element is to be deleted, meaning the "next" iteration should check the element that will take i's place
			std::cout << std::string("Colisionaron ") << conflictingMines.size() << std::string(" minas en ") << conflictingMines[0].position.xpos << std::string(", ") << conflictingMines[0].position.ypos << std::endl; // conflictingMines will always have a value at [0]
			for (Mine const &mine: conflictingMines) {
				removeMine(board, mine);
				wasThereCollision = true;
			}
		}
		conflictingMines.clear();
	}
	return wasThereCollision;
}