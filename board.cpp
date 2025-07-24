#include "board.h"

bool Position::operator==(const Position &b) const {
	return ((xpos == b.xpos) && (ypos == b.ypos));
}

bool Mine::operator==(const Mine &b) const {
	return ((position.xpos == b.position.xpos) && (position.ypos == b.position.ypos)) && (owner == b.owner);
}

bool Mine::operator==(const Position &b) const {
	return ((position.xpos == b.xpos) && (position.ypos == b.ypos));
}

Board::~Board() {
	//used to have memory deallocation
	//preserving just in case
}

bool isPositionValid(Board const &board, Position const &pos) { // check if pos if withing acceptable values
	// If the position is outside the board, return false;
	if ((pos.xpos < 1) || (pos.ypos < 1) || (pos.xpos > board.width) || (pos.ypos > board.height)) {
		return false;
	}
	// if the position is disabled, return false
	for (auto it = board.disabledPositions.begin(); it != board.disabledPositions.end(); it++) {
		if (pos == *it) {
			return false;
		}
	}
	// else, return true
	return true;
}

Player getPlayer(Board const &board, int playerID) {
	return board.players[playerID - 1];
}
// gets all possible valid positions
// mainly for bot logic
std::vector<Position> getValidTiles(Board const &board) {
	std::vector<Position> validPositions;
	for (unsigned int x = 1; x <= board.width; x++) {
		for (unsigned int y = 1; y <= board.height; y++) {
			Position pos{x, y};
			if (isPositionValid(board, pos)) {
				validPositions.push_back(std::move(pos));
			}
		}
	}
	return validPositions;
}

void disablePosition(Board &board, Position const &disabledPosition) {
	Position pos = disabledPosition;
	board.disabledPositions.insert(pos);
}

void disablePosition(Board &board, Mine const &disabledMine) {
	Position pos = disabledMine.position;
	board.disabledPositions.insert(pos);
}

bool removeMine(Board &board, Mine mine) {
	for (std::vector<Mine>::iterator it = board.placedMines.begin(); it != board.placedMines.end(); it++) {
		if (*it == mine) {
			board.placedMines.erase(it);
			board.players[mine.owner].mineCount--;
			disablePosition(board, mine);
			return true;
		}
	}
	return false; // just in case I need to check if a mine was properly erased or not
}

// function to be called at the end of each turn
void disableTilesUsed(Board &board) {
	for (std::vector<Mine>::iterator it = board.placedMines.begin(); it != board.placedMines.end(); it++) {
		disablePosition(board, *it);
	}
}

void eliminatePlayers(Board &board) {
	int newPlayerCount = board.playerCount;
	std::vector<Player> newPlayerList;
	std::cout << "eliminating players " << newPlayerCount << std::endl;
	for (int i = 0; i < board.playerCount; i++) {
		std::cout << "player " << i << std::endl;
		Player player = board.players[i];
		if (player.mineCount <= 0) { // player is removed when they have no mines remaining
			std::cout << "removed " << i << std::endl;
			newPlayerCount--;
		} else {
			std::cout << "preserved " << i << std::endl;
			newPlayerList.push_back(player);
			std::cout << "pushed to new vector " << i << std::endl;
		}
	}
	std::cout << "creating new player array of size" << newPlayerCount << std::endl;
	std::vector<Player> players(newPlayerCount); // build new player array without the removed players
	for (int i = 0; i < newPlayerCount; i++) {
		players[i] = newPlayerList[i];
	}
	board.players = players;
	board.playerCount = newPlayerCount;
}
// when only one player has mines remaining, they win the game
// if no players have mines, game is a draw
int gameEndCondition(Board &board) {
	if (board.playerCount == 1) return board.players[0].id; // player wins if they're the only one remaining
	if (board.playerCount == 0) return 0;					// if no players remain, game is a draw

	// check if there are enough tiles to accomodate every player's mines, if not, the game is a draw
	int maxPlayerMines = 0;
	for (int i = 0; i < board.playerCount; i++) {
		if (board.players[i].mineCount > maxPlayerMines) {
			maxPlayerMines = board.players[i].mineCount;
		}
	}
	int tilesRemaining = getValidTiles(board).size();
	std::cout << "tiles remaining: " << tilesRemaining << std::endl;
	if (tilesRemaining < maxPlayerMines) return 0;

	return -1; // else, the game is not over
}
