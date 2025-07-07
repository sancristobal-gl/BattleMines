#include <vector>
#include <iostream>
#include <string>
#include <cstdlib> 

struct Position{
    int xpos;
    int ypos;

    bool operator==(const Position b) const{
        return ((xpos == b.xpos)
            && (ypos == b.ypos));
    }
    Position(){

    }
    Position(int x, int y){
        this->xpos = x;
        this->ypos = y;
    }
};

struct Mine : Position{
    int owner;

    bool operator==(const Mine b) const{
        return ((xpos == b.xpos)
            && (ypos == b.ypos))
            && (owner == b.owner);
    }

    bool operator==(const Position b) const{
        return ((xpos == b.xpos)
            && (ypos == b.ypos));
    }
};

struct Player{
    int id;
    Mine* mines;
    int mineCount;
    bool operator==(const Player b){
        return ((id == b.id));
    }
    bool isAI;
};

struct Board{
    int width;
    int height;
    std::vector<Position> disabledPositions;
    Player* players;
    int playerCount;
    std::vector<Mine> placedMines;
    bool pve;
};
bool isPositionValid(Board board, Position pos);

std::vector<Position> getValidTiles(Board board);

Board createBoard();

void printField(Board board, int perspective = -1);

void disablePosition(Board &board, Position disabledPosition);

bool removeMine(Board &board, Mine mine);

void disableTilesUsed(Board &board);

int checkWinCon(Board board);