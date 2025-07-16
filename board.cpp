#include "board.h"
const int undefinedPerspective = -1;

bool Position::operator==(const Position &b) const{
    return ((xpos == b.xpos)
        && (ypos == b.ypos));
}

bool Mine::operator==(const Mine &b) const{
    return ((position.xpos == b.position.xpos)
        && (position.ypos == b.position.ypos))
        && (owner == b.owner);
}

bool Mine::operator==(const Position &b) const{
    return ((position.xpos == b.xpos)
        && (position.ypos == b.ypos));
}

Board::~Board(){
    delete[] players;
}

bool isPositionValid(Board const& board, Position const& pos){ //check if pos if withing acceptable values 
    //If the position is outside the board, return false;
    if((pos.xpos < 1)
    || (pos.ypos < 1)
    || (pos.xpos > board.width)
    || (pos.ypos > board.height)) return false;
    //if the position is disabled, return false
    for(int i = 0; i < board.disabledPositions.size(); i++){
        if(pos == board.disabledPositions[i]) return false;
    }
    //else, return true
    return true;
}

//gets all possible valid positions
//mainly for bot logic
std::vector<Position> getValidTiles(Board const& board){ 
    std::vector<Position> validPositions;
    for(int x = 1; x <= board.width; x++){
        for(int y = 1; y <= board.height; y++){
            Position pos;
            pos.xpos = x;
            pos.ypos = y;
            if (isPositionValid(board, pos)){
                validPositions.push_back(pos);
            }
        }
    }
    return validPositions;
}

int getValuesWithinRange(std::string const& prompt, int min, int max){ //function to input an int value within range, used for board initialization, probably could be reporopused for other uses
    int ret = min-1;
    while ((ret < min) || (ret > max)){
        std::cout << prompt << std::endl;
        std::cout << "value must be between " << min << " and " << max << std::endl;
        std::cin >> ret;
    }
    return ret;
    
}
//board construction
Board createBoard(){
    Board board;
    bool created = false;
    int width = NULL;
    int height = NULL;
    int mineCount = 0;
    board.gameType = static_cast<gameType>(getValuesWithinRange("choose game mode (0=PVP, 1=PVE)", 0, 1)); //not sure if this is good practice
    board.width = getValuesWithinRange("choose the width of the field", 5, 10);
    board.height = getValuesWithinRange("choose the height of the field", 5, 10);
    mineCount = getValuesWithinRange("choose the number of mines on the field", 3, 8);
    board.playerCount = getValuesWithinRange("choose the number of mines on the field", 2, 8);
    board.players = new Player[board.playerCount]; //dynamic array since we don't know the number of players beforehand
    for(int p = 0; p < board.playerCount; p++){
        Player player;
        player.mineCount = mineCount;
        player.id = p+1;
        if(board.gameType == PVE){
            if(p > 1){
                player.isAI = true;
            }
        }
        board.players[p] = player;
        }
    return board;
}

namespace printBoardAuxiliars{
    void showPositionStatus(Board const& board, unsigned int x, unsigned int y, int perspective = undefinedPerspective);

    void printRow(Board const& board, unsigned int y);

    void printColumnInRow(Board const& board, unsigned int x, unsigned int y, int perspective);
}

void printBoardAuxiliars::showPositionStatus(Board const& board, unsigned int x, unsigned int y, int perspective){
    //print the status of the position {x, y}
    //(" " = no existe, "O" = posicion en juego/estado desconocido, "M" = mina del jugador en la posicion)
    bool isPositionEnabled = true;
    
    for(int i = 0; i < board.disabledPositions.size(); i++){
        Position disabledPos = board.disabledPositions[i];
        Position pos = {x, y};
        if(disabledPos == pos){
            isPositionEnabled = false;
            break;
        }
    }
    bool mineInPos=false;
    if (perspective != undefinedPerspective){ //show ony the mines belonging to player
        for(std::vector<Mine>::const_iterator it = board.placedMines.begin(); it != board.placedMines.end(); it++){
            if(it->position.xpos == x && it->position.ypos == y){
                if(it->owner == perspective){
                    mineInPos=true;
                    break;
                }
            }
        }
    }
    if(mineInPos == true) std::cout << "  M";
    else if(isPositionEnabled==false) std::cout << "   ";
    else if(isPositionEnabled == true) std::cout << "  O";
}

void printBoardAuxiliars::printRow(Board const& board, unsigned int y){
    int spaceLength = (3 - std::to_string(y).length());
    if (y == 0){ //si es la primera fila, imprimimos solo espacio
            std::cout << "   ";
        }
    else{ //caso contrario, imprimimos el numero de la fila
        std::cout << y;
        for(int space = 0; space < spaceLength; space++){
            std::cout << ' ';
        };
    }
}

void printBoardAuxiliars::printColumnInRow(Board const& board, unsigned int x, unsigned int y, int perspective){
    int spaceLength = (3 - std::to_string(x).length());
    if (y == 0){ //si es la primera fila, imprimimos los numeros de las columnas
        for(int space = 0; space < spaceLength; space++){
            std::cout << ' ';
        };
        std::cout << std::to_string(x);
    }
    else{ //en caso contrario, imprimimos el estado de la posicion 
        printBoardAuxiliars::showPositionStatus(board, x, y, perspective);
    }
    return;
}

//print board to console
//may be depreciated if an UI is implemented in the future
void printBoard(Board const& board, int perspective){
    for (int y = 0; y <= board.height; y++){ //para cada fila
        printBoardAuxiliars::printRow(board, y);
        for (int x = 1; x <= board.width; x++){ // seguido del numero de fila, imprimimos todas las posiciones de la fila
            printBoardAuxiliars::printColumnInRow(board, x, y, perspective);
        }
        std::cout << std::endl;
    }
}

void disablePosition(Board &board, Position const& disabledPosition){
    board.disabledPositions.push_back(disabledPosition);
}

void disablePosition(Board &board, Mine const& disabledMine){
    board.disabledPositions.push_back(disabledMine.position);
}

bool removeMine(Board &board, Mine mine){
    for(std::vector<Mine>::iterator it = board.placedMines.begin(); it != board.placedMines.end(); it++){
        if (*it == mine){
            board.placedMines.erase(it);
            board.players[mine.owner].mineCount--;
            std::cout << "Player " << mine.owner + 1 << " mines remaining: " << board.players[mine.owner].mineCount << std::endl;
            disablePosition(board, mine);
            return true;
        }
    }
    return false; //just in case I need to check if a mine was properly erased or not
}

//function to be called at the end of each turn
void disableTilesUsed(Board &board){ 
    for(std::vector<Mine>::iterator it = board.placedMines.begin(); it != board.placedMines.end(); it++){
        disablePosition(board, *it);
    }
}

//TODO: Refactor when board.players is changed from array to std::vector
void eliminatePlayers(Board& board){
    int newPlayerCount = board.playerCount;
    std::vector<Player> newPlayerList;
    for(int i = 0; i<board.playerCount; i++){
        if (board.players[i].mineCount <= 0){
            newPlayerCount--;
        }
        else{
            newPlayerList.push_back(board.players[i]);
        }
    }
    Player* players = new Player[newPlayerCount];
    for(int i = 0; i<newPlayerCount; i++){
        players[i] = newPlayerList[i];
    }
    delete[] board.players;
    board.players = players;
    board.playerCount = newPlayerCount;
}
//when only one player has mines remaining, they win the game
//if no players have mines, game is a draw
//TODO: refactor to support more than 2 players
//Should make it so that players without mines are eliminated and the other players remain in the game
int gameEndCondition(Board & board){
    if(board.playerCount == 1) return board.players[0].id; //player wins if they're the only one remaining
    if(board.playerCount == 0) return 0; //if no players remain, game is a draw

    //check if there are enough tiles to accomodate every player's mines, if not, the game is a draw
    int maxPlayerMines = 0;
    for(int i = 0; i < board.playerCount; i++){
        if(board.players[i].mineCount < maxPlayerMines) maxPlayerMines = board.players[i].mineCount;
    }
    if(getValidTiles(board).size() < maxPlayerMines) return 0;

    return -1; //else, the game is not over
}
