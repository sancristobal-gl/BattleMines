#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime>
#include <vector>
#include <unordered_map>
#include <utility>
#include "stages.h"

int main(){
    srand(time(0)); // initialize random number generator
    Board board = createBoard();
    int winner = gameLoop(board);
    std::cout << "The winner is: player " << winner+1 << "! Congratulations!" << std::endl;
}