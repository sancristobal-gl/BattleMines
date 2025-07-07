#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime>
#include <vector>
#include <unordered_map>
#include <utility>
#include "stages.h"

int gameLoop(Board &board){
    /*
    winner = 0, empate
    winner = 1, gano jugador 1
    winner = 2, gano jugador 2
    */
    int winner = -1; 
    while (winner == -1){
        roundStartStage(board);
        winner = minePlacementStage(board);
        if(winner != -1) return winner;
        winner = guessingStage(board);
        if(winner != -1) return winner;
        roundEndStage(board);
    }
    return winner;
    return 0;
}

int main(){
    srand(time(0)); // initialize random number generator
    Board board = createBoard();
    int winner = gameLoop(board);
    std::cout << "The winner is: player " << winner << "! Congratulations!" << std::endl;
}