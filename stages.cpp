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
    std::vector<Position> asd = getValidTiles(board);
    for(int i = 0; i < asd.size(); i++){
        std::cout << asd[i].xpos << ", " << asd[i].ypos << std::endl;
    }
    for(int p = 0; p < board.playerCount; p++){
        std::cout << "Player " << board.players[p].id + 1 << "'s turn to place their mines:" << std::endl;
        chooseMinePositions(board, board.players[p]);
    }
    bool wasThereCollision = chechMineCollision(board);
    if (wasThereCollision == true){
        system("pause");
        system("cls");
    }
    return checkWinCon(board);
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
    disableTilesUsed(board);
    board.placedMines.clear();
}