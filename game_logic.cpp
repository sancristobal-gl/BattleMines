
#include "game_logic.h"

std::string getPlayerInput(Board board, Player player, int askCoordinate = 0){
    std::string ret = "";
    if(player.isAI == false){
        std::cin >> ret;
        return ret;
    }
    else{
        std::vector<Position> validTiles = getValidTiles(board);
        int randomPos = rand() % validTiles.size();
        if(askCoordinate == 0){
            ret = std::to_string(validTiles[randomPos].xpos);
        }
        else{
            ret = std::to_string(validTiles[randomPos].ypos);
        }
        system("cls");
    }
    return ret;
}

void chooseMinePositions(Board &board, Player &player){
    for(int m = 0; m < player.mineCount; m++){
        printField(board, player.id);
        bool validPlacement = false;
        Mine mine;
        while(validPlacement == false){
            std::cout << "Player " << player.id << "!, choose your mine's positions" << std::endl;
            std::cout << "Choose the x position of mine " << m+1 << ": ";
            mine.xpos = std::stoi(getPlayerInput(board, player, 0));;
            std::cout << "Choose the y position of mine " << m+1 << ": ";
            mine.ypos = std::stoi(getPlayerInput(board, player, 1));;
            mine.owner = player.id;
            validPlacement = isPositionValid(board, mine);
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
        std::cout << "Player " << mine.owner+1 <<  " placed mine at " << mine.xpos << ", " << mine.ypos << std::endl;
        board.placedMines.push_back(mine);
        std::cout << std::endl;
        system("cls");

    }
}

void guess(Board &board, Player player){
    printField(board, player.id);
    Position guess;
    int isGuessValid = false; //flag to check if the inputed position is valid. If not, ask the player again
    while(isGuessValid == false){
        std::cout << "Player " << player.id <<  ", take a guess... " << std::endl;
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
    for(std::vector<Mine>::iterator it = board.placedMines.begin(); it != board.placedMines.end(); it++){
        if (*it == guess){
            if(it->owner == player.id){
                std::cout << "That's your own mine, silly!"<< std::endl; //el jugador pierde la oportunidad de volver a adivinar
            }
            else{
                removeMine(board, *it);
                std::cout << "You found an enemy mine!" <<  std::endl;
                disablePosition(board, guess);
                system("pause");
            }
        }
        else disablePosition(board, guess);
    } 
}

bool chechMineCollision(Board &board){
    bool wasThereCollision = true;
    std::vector<Mine> conflictingMines;
    for(int i = 0; i<board.placedMines.size(); i++){
        conflictingMines.push_back(board.placedMines[i]);
        for(int j = i+1; j<board.placedMines.size(); j++){
            if(board.placedMines[i].xpos == board.placedMines[j].xpos && board.placedMines[i].ypos == board.placedMines[j].ypos){ //they'll be equal if they share the same positoon, the owner is not a factor
                conflictingMines.push_back(board.placedMines[j]);
            }
        }
        if(conflictingMines.size() > 1){
            std::cout << "Colisionaron minas en " << conflictingMines[0].xpos << ", " << conflictingMines[0].ypos << std::endl; //conflictingMines siempre tendra un valor en [0]
            for(Mine mine : conflictingMines){
                removeMine(board, mine);
                wasThereCollision = true;
            }
        }
        conflictingMines.clear();
    }
    return wasThereCollision;
    /*
    typedef std::unordered_map<std::pair<int, int>, std::vector<int>> mineMap;
    
    mineMap mineOwners; //map of positions(respresented as a pair of ints) and a vector of ints that represent the players who put a mine there
    
    for(int p = 0; p<board.playerCount; p++){
        for(int m = 0; m<board.players[p].mineCount; m++){
            std::pair <int, int> tempPair = std::make_pair(board.players[p].mines[m].xpos, board.players[p].mines[m].ypos);
            mineOwners[tempPair].push_back(p);
        }
    }
    bool wasThereCollision = false;
    for(mineMap::iterator it = mineOwners.begin(); it != mineOwners.end(); it++){
        if (it->second.size() > 1){
            wasThereCollision = true;
            std::cout << "Mines collided at " << it->first.first << ", " << it->first.second << std::endl;
            for (int p : it->second){
                removeMine(board.players[p], Position(it->first.first, it->first.second));
            }
        }
    }
    return wasThereCollision;
    */
}

int game(Board &board){
    /*
    winner = 0, empate
    winner = 1, gano jugador 1
    winner = 2, gano jugador 2
    */
    int winner = -1; 
    while (winner == -1){
        //system("cls");
        for(int p = 0; p < board.playerCount; p++){
            std::cout << "Player " << board.players[p].id << " mine count: " << board.players[p].mineCount << std::endl;
        }
        printField(board);
        std::cout << "Press enter to commence the round!" << std::endl;
        system("pause");
        system("cls");
        for(int p = 0; p < board.playerCount; p++){
            std::cout << "Player " << board.players[p].id << "'s turn to place their mines:" << std::endl;
            chooseMinePositions(board, board.players[p]);
        }
        bool wasThereCollision = chechMineCollision(board);
        winner = checkWinCon(board);
        if(winner != -1) return winner;

        if (wasThereCollision == true){
            system("pause");
            system("cls");
        }
        for(int p = 0; p < board.playerCount; p++){
            std::cout << "Player " << board.players[p].id << "'s turn to guess:" << std::endl;
            guess(board, board.players[p]);
            //system("cls");
        }
        winner = checkWinCon(board);
        if(winner != -1) return winner;
        
        disableTilesUsed(board);
        board.placedMines.clear();
        
        //TODO: make win condition function compatible with player count > 2
        
    }
    return 0;
}