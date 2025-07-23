#include <bitset> //for debugging
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Position {
	unsigned int xpos = 0;
	unsigned int ypos = 0;

	bool operator==(const Position &pos) const;
};

struct PositionHash { // hashing implementation for Position
	size_t operator()(const Position &pos) const {
		// debugging;
		//std::bitset<64> ea(((long long)pos.xpos << 32) ^ (long long)pos.ypos);
		//std::cout << ea << std::endl;
		return std::hash<long long>()(((long long)pos.xpos << 32) ^ (long long)pos.ypos); // xpos and ypos are 32bit unsigned integers, we concatenate them into a 64bit long long to create a key
	}
};

struct Mine {
	Position position;

	int owner = -1;

	bool operator==(const Mine &b) const;

	bool operator==(const Position &b) const;
};

struct Player {
	int id = -1;
	Mine *mines = nullptr;
	int mineCount = 0;
	bool operator==(const Player b) {
		return (id == b.id);
	}
	bool isAI = false;
};

enum gameType {
	PVP,
	PVE
};

struct Board {
	int width = 0;
	int height = 0;
	std::unordered_set<Position, PositionHash> disabledPositions;
	Player *players = nullptr;
	int playerCount = 0;
	std::vector<Mine> placedMines;
	gameType gameType = PVP;
	~Board();
};

bool isPositionValid(Board const &board, Position const &pos);

std::vector<Position> getValidTiles(Board const &board);

void disablePosition(Board &board, Position const &disabledPosition);

bool removeMine(Board &board, Mine mine);

void disableTilesUsed(Board &board);

void eliminatePlayers(Board &board);

int gameEndCondition(Board &board);