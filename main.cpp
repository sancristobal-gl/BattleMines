#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime>
#include <vector>
#include <unordered_map>
#include <utility>
#include "game_logic.h"



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
int main(){
    srand(time(0)); // initialize random number generator
    Board board = createBoard();
    int winner = game(board);
    std::cout << "The winner is: player " << winner << "! Congratulations!" << std::endl;
}