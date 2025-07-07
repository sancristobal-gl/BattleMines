#include "stages.h"

void roundStartStage(Board board){
    for(int p = 0; p < board.playerCount; p++){
        std::cout << "Player " << board.players[p].id + 1 << " mine count: " << board.players[p].mineCount << std::endl;
    }
    printField(board);
    std::cout << "Press enter to commence the round!" << std::endl;
    system("pause");
    system("cls");
}

int minePlacementStage(Board &board){
    for(int p = 0; p < board.playerCount; p++){
        std::cout << "Player " << board.players[p].id + 1 << "'s turn to place their mines:" << std::endl;
        chooseMinePositions(board, board.players[p]);
    }
    bool wasThereCollision = chechMineCollision(board); //check if mines collide, if they do, notify the player
    if (wasThereCollision == true){
        system("pause");
        system("cls");
    }
    return checkWinCon(board); //every end step, check if a winner has been decided
}

int guessingStage(Board &board){
    for(int p = 0; p < board.playerCount; p++){
        std::cout << "Player " << board.players[p].id + 1 << "'s turn to guess:" << std::endl;
        guess(board, board.players[p]);
        system("cls");
    }
    return checkWinCon(board);
}

void roundEndStage(Board &board){
    disableTilesUsed(board); //tiles that were in play during this turn will become unavailable for the rest of the game
    board.placedMines.clear(); //clear all mines from the field
}