#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

struct Position {
	unsigned int xpos = 0;
	unsigned int ypos = 0;

	bool operator==(const Position &b) const;
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
	std::vector<Position> disabledPositions;
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