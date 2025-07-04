#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime>
#include <vector>
#include <unordered_map>
#include <utility>
#include "game_logic.h"
#include "stages.h"

int main(){
    srand(time(0)); // initialize random number generator
    Board board = createBoard();
    int winner = game(board);
    std::cout << "The winner is: player " << winner << "! Congratulations!" << std::endl;
}