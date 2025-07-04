#include "board.h"

bool isPositionValid(Board board, Position pos){ //check if pos if withing acceptable values 
    if(pos.xpos < 0 || pos.ypos < 0) return false;
    if(pos.xpos > board.width || pos.ypos > board.height) return false;
    for(std::vector<Position>::iterator it = board.disabledPositions.begin(); it != board.disabledPositions.end(); it++){ //check if pos is not equal to an invalid position
        if(*it == pos){
            return false;
        }
    }
    return true;
}

//gets all possible valid positions
//mainly for bot logic
std::vector<Position> getValidTiles(Board board){ 
    std::vector<Position> validPositions;
    for(int x = 0; x < board.width; x++){
        for(int y = 0; y < board.height; y++){
            Position pos;
            pos.xpos = x;
            pos.ypos = y;
            bool isValid = true;
            for(std::vector<Position>::iterator it = board.disabledPositions.begin(); it != board.disabledPositions.end(); it++){
                if(isPositionValid(board, *it)) isValid = false;
            }
            if (isValid){
                validPositions.push_back(pos);
            }
        }
    }
    return validPositions;
}

//board construction
Board createBoard(){
    Board board;
    bool created = false;
    int width = NULL;
    int height = NULL;
    int mineCount = 0;
    std::string pvx = "";
    while (!(pvx == "pve") && !(pvx == "pvp")){
        std::cout << "will this be a player vs player game or a player vs AI game?" << std::endl;
        std::cout << "write 'pvp' for player vs player or 'pve' for player vs AI" << std::endl;
        std::cin >> pvx;
    }
    while ((25 > width) || (width > 50)){
        std::cout << "choose the width of the field (between 24 and 50)" << std::endl;
        std::cin >> width;
    }
    while ((25 > height) || (height > 50)){
        std::cout << "choose the height of the field (between 24 and 50)" << std::endl;
        std::cin >> height;
    }
    while ((3 > mineCount) || (mineCount > 8)){
        std::cout << "choose the number of mines on the field (between 3 and 8)" << std::endl;
        std::cin >> mineCount;
    }
    board.width = width;
    board.height = height;
    board.playerCount = 2;
    board.players = new Player[board.playerCount]; //dynamic array since we don't know the number of players beforehand
    for(int p = 0; p < board.playerCount; p++){
        Player player;
        player.mineCount = mineCount;
        player.id = p;
        if(p==1 && pvx == "pve"){ //set player 2 to be controlled by AI, may need to be refactored when support for more than 2 players is implemented
            player.isAI = true;
        }
        board.players[p] = player;
        
        std::cout << "created player " << p << std::endl;
    }
    return board;
}

//print board to console
//may be depreciated if an UI is implemented in the future
void printField(Board board, int perspective){ //no se yo tampoco entiendo nada
    bool isPositionEnabled;
    for (int y = 0; y <= board.height; y++){ //para cada fila
        if (y == 0){ //si es la primera fila, imprimimos solo espacio
            std::cout << "   ";
        }
        else{ //caso contrario, imprimimos el numero de la fila
            std::cout << y;
            for(int space = 0; space < (3 - std::to_string(y).length()); space++){
                std::cout << " ";
            };
        }
        for (int x = 1; x <= board.width; x++){ // seguido del numero de fila, imprimimos todas las posiciones de la fila
            if (y == 0){ //si es la primera fila, imprimimos los numeros de las columnas
                for(int space = 0; space < (3 - std::to_string(x).length()); space++){
                    std::cout << " ";
                };
                std::cout << std::to_string(x);
            }
            else{ //en caso contrario, imprimimos el estado de la posicion (" " = no existe, "O" = posicion en juego/estado desconocido, "M" = mina del jugador en la posicion)
                isPositionEnabled = true;
                for(std::vector<Position>::iterator it = board.disabledPositions.begin(); it != board.disabledPositions.end(); it++){
                    if(it->xpos == x && it->ypos == y){
                        isPositionEnabled = false;
                    }
                }
                bool mineInPos=false;
                if (perspective != -1){ //shows the mines belonging to player
                    for(std::vector<Mine>::iterator it = board.placedMines.begin(); it != board.placedMines.end(); it++){
                        if(it->xpos == x && it->ypos == y){
                            if(it->owner == perspective){
                                std::cout << "  M";
                                mineInPos=true;
                            }
                        }
                    }
                }
                if(mineInPos == false){
                    if(isPositionEnabled==false){
                        std::cout << "   ";
                    }
                    else if(isPositionEnabled == true){
                        std::cout << "  O";
                    }
                }
            }
        }
        std::cout << std::endl;
    }
}

void disablePosition(Board &board, Position disabledPosition){
    board.disabledPositions.push_back(disabledPosition);
}

bool removeMine(Board board, Mine mine){
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

//when only one player has mines remaining, they win the game
//if no players have mines, game is a draw
//TODO: refactor to support more than 2 players
int checkWinCon(Board board){
    int winner = -1;
    if(board.players[0].mineCount <= 0){
        if(board.players[1].mineCount <= 0){
            winner = 0;
        }
        else winner=2;
    }
    if(board.players[1].mineCount <= 0){
        winner=1;
    }
    return winner;
}
