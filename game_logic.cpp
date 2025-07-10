#include "game_logic.h"

//gets input from console if player is human
//generates random input if player is bot
std::string getPlayerInput(Board &board, Player player, int askCoordinate = 0){
    std::string ret = "";
    if(player.isAI == false){
        std::cin >> ret;
        return ret;
    }
    else{
        std::vector<Position> validTiles = getValidTiles(board); //TODO: make bot not be able to choose the positions where their own mines are placed
        int randomPos = rand() % validTiles.size();
        if(askCoordinate == 0){
            ret = std::to_string(validTiles[randomPos].xpos);
        }
        else{
            ret = std::to_string(validTiles[randomPos].ypos);
        }
        std::cout << std::endl;
    }
    return ret;
}

void chooseMinePositions(Board &board, Player &player){
    for(int m = 0; m < player.mineCount; m++){
        printBoard(board, player.id);
        bool validPlacement = false;
        Mine mine;
        while(validPlacement == false){
            Position pos;
            std::cout << "Player " << player.id + 1 << "!, choose your mine's positions" << std::endl;
            std::cout << "Choose the x position of mine " << m+1 << ": ";
            pos.xpos = std::stoi(getPlayerInput(board, player, 0));;
            std::cout << "Choose the y position of mine " << m+1 << ": ";
            pos.ypos = std::stoi(getPlayerInput(board, player, 1));;
            mine.owner = player.id;
            mine.position = pos;
            validPlacement = isPositionValid(board, pos);
            if(validPlacement==false){
                std::cout << "Invalid position! Choose again" << std::endl;
            }
            for(std::vector<Mine>::iterator it = board.placedMines.begin(); it!= board.placedMines.end(); it++){
                if (*it == mine){ //for them to be equal, they need to share position and owner
                    std::cout << "You already placed a mine there! Choose again" << std::endl;
                    validPlacement = false;
                }
            }
            
        }
        //std::cout << "Player " << mine.owner+1 <<  " placed mine at " << mine.xpos << ", " << mine.ypos << std::endl; //for testing purposes, TODO: remove before main release
        board.placedMines.push_back(mine);
        std::cout << std::endl;
        system("cls");

    }
}

void guess(Board &board, Player & player){
    printBoard(board, player.id);
    Position guess;
    int isGuessValid = false; //flag to check if the inputed position is valid. If not, ask the player again
    while(isGuessValid == false){
        std::cout << "Player " << player.id+1 <<  ", take a guess... " << std::endl;
        std::cout << "X position: ";
        guess.xpos = std::stoi(getPlayerInput(board, player, 0));;
        std::cout << std::endl;
        std::cout << "Y position: ";
        guess.ypos = std::stoi(getPlayerInput(board, player, 1));
        std::cout << std::endl;
        isGuessValid = isPositionValid(board, guess);
        if(isGuessValid == false){
            std::cout << "That spot has already been checked! Try again" << std::endl;
        }
        //system("cls");
    }
    //check if the guessed position shares the same position as a mine
    std::vector<Mine> minesToRemove;
    for(std::vector<Mine>::iterator it = board.placedMines.begin(); it != board.placedMines.end(); it++){
        if (*it == guess){
            if(it->owner == player.id){
                std::cout << "That's your own mine, silly!"<< std::endl; //el jugador pierde la oportunidad de volver a adivinar
            }
            else{
                removeMine(board, *it);
                disablePosition(board, guess);
                std::cout << "You found an enemy mine!" <<  std::endl;
                system("pause");
                return;
            }
        }
        else disablePosition(board, guess);
    }
    return;
}

//function to be called after each player has placed their mines
//colliding mines are annihilated and removed from their owner's mine pool
bool chechMineCollision(Board &board){
    bool wasThereCollision = true;
    std::vector<Mine> conflictingMines;
    for(int i = 0; i<board.placedMines.size(); i++){
        Position mine1Pos = board.placedMines[i].position;
        conflictingMines.push_back(board.placedMines[i]);
        for(int j = i+1; j<board.placedMines.size(); j++){
            Position mine2Pos = board.placedMines[j].position;
            if(mine1Pos.xpos == mine2Pos.xpos && mine1Pos.ypos == mine2Pos.ypos){ //they'll be equal if they share the same positoon, the owner is not a factor
                conflictingMines.push_back(board.placedMines[j]);
            }
        }
        if(conflictingMines.size() > 1){
            i--;
            std::cout << "Colisionaron minas en " << conflictingMines[0].position.xpos << ", " << conflictingMines[0].position.ypos << std::endl; //conflictingMines will always have a value at [0]
            for(Mine mine : conflictingMines){
                removeMine(board, mine);
                wasThereCollision = true;
            }
        }
        conflictingMines.clear();
    }
    return wasThereCollision;
}