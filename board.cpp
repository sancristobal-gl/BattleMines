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
	delete[] players;
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
	std::cout << "DEBUG disabled position (pos)" << std::endl;
}

void disablePosition(Board &board, Mine const &disabledMine) {
	Position pos = disabledMine.position;
	board.disabledPositions.insert(pos);
	std::cout << "DEBUG disabled position (mine)" << std::endl;
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

// TODO: Refactor when board.players is changed from array to std::vector
void eliminatePlayers(Board &board) {
	int newPlayerCount = board.playerCount;
	std::vector<Player> newPlayerList;
	for (int i = 0; i < board.playerCount; i++) {
		if (board.players[i].mineCount <= 0) {
			newPlayerCount--;
		} else {
			newPlayerList.push_back(board.players[i]);
		}
	}
	Player *players = new Player[newPlayerCount];
	for (int i = 0; i < newPlayerCount; i++) {
		players[i] = newPlayerList[i];
	}
	delete[] board.players;
	board.players = players;
	board.playerCount = newPlayerCount;
}
// when only one player has mines remaining, they win the game
// if no players have mines, game is a draw
// TODO: refactor to support more than 2 players
// Should make it so that players without mines are eliminated and the other players remain in the game
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
