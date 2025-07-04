#include "stages.h"

int roundStartStage(Board board){
    for(int p = 0; p < board.playerCount; p++){
        std::cout << "Player " << board.players[p].id << " mine count: " << board.players[p].mineCount << std::endl;
    }
    printField(board);
    std::cout << "Press enter to commence the round!" << std::endl;
    system("pause");
    system("cls");
}

int minePlacementStage(Board board, int winner){
    for(int p = 0; p < board.playerCount; p++){
        std::cout << "Player " << board.players[p].id << "'s turn to place their mines:" << std::endl;
        chooseMinePositions(board, board.players[p]);
    }
    bool wasThereCollision = chechMineCollision(board);
    if (wasThereCollision == true){
        system("pause");
        system("cls");
    }
    winner = checkWinCon(board);
    return winner;
}

int guessingStage(Board board, int winner){
    for(int p = 0; p < board.playerCount; p++){
        std::cout << "Player " << board.players[p].id << "'s turn to guess:" << std::endl;
        guess(board, board.players[p]);
        //system("cls");
    }
    winner = checkWinCon(board);
    return winner;
}

void roundEndStage(Board board){
    disableTilesUsed(board);
    board.placedMines.clear();
}

int gameLoop(Board &board){
    /*
    winner = 0, empate
    winner = 1, gano jugador 1
    winner = 2, gano jugador 2
    */
    int winner = -1; 
    while (winner == -1){
        roundStartStage(board);
        winner = minePlacementStage(board, winner);
        winner = guessingStage(board, winner);
        roundEndStage(board);
    }
    return 0;
}