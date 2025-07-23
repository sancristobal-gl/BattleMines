#include "board_console_implementation.h"

const int undefinedPerspective = -1; // perspective represent the player who the board is being printed for
// a player should be able to see the position of their own hidden mines but not of the other players

const int charsPerLabel = 3;

namespace boardConsoleDisplayHelper {
	void showPositionStatus(Board const &board, unsigned int x, unsigned int y, int perspective = undefinedPerspective);

	void printRow(Board const &board, unsigned int y);

	void printColumnInRow(Board const &board, unsigned int x, unsigned int y, int perspective);
}

void printToPlayer(Player player, std::string message){ //shows message on console, unless the player is AI, to avoid spammig the console and potentially crashing the program
	if (!player.isAI){
		std::cout << message << std::endl;
	}
	
}
void boardConsoleDisplayHelper::showPositionStatus(Board const &board, unsigned int x, unsigned int y, int perspective) {
	// print the status of the position {x, y}
	//(" " = doesn't exist, "O" = valid position with unknown contents, "M" = player mine in position)
	bool isPositionEnabled = true;

	for (auto it = board.disabledPositions.begin(); it != board.disabledPositions.end(); it++) {
		Position disabledPos = *it;
		Position pos = {x, y};
		if (disabledPos == pos) {
			isPositionEnabled = false;
			break;
		}
	}
	bool mineInPos = false;
	if (perspective != undefinedPerspective) { // show ony the mines belonging to player
		for (std::vector<Mine>::const_iterator it = board.placedMines.begin(); it != board.placedMines.end(); it++) {
			if (it->position.xpos == x && it->position.ypos == y) {
				if (it->owner == perspective) { // id is player position in array + 1
					mineInPos = true;
					break;
				}
			}
		}
	}
	if (mineInPos == true)
		std::cout << "  M";
	else if (isPositionEnabled == false)
		std::cout << "   ";
	else if (isPositionEnabled == true)
		std::cout << "  O";
}

void boardConsoleDisplayHelper::printRow(Board const &board, unsigned int y) {
	int spaceLength = (charsPerLabel - std::to_string(y).length());
	if (y == 0) { // if is first row, we print empty space
		std::cout << "   ";
	} else { // else, print row label
		std::cout << y;
		for (int space = 0; space < spaceLength; space++) {
			std::cout << ' ';
		};
	}
}

void boardConsoleDisplayHelper::printColumnInRow(Board const &board, unsigned int x, unsigned int y, int perspective) {
	int spaceLength = (charsPerLabel - std::to_string(x).length());
	if (y == 0) { // if it's first column, we print column label
		for (int space = 0; space < spaceLength; space++) {
			std::cout << ' ';
		};
		std::cout << std::to_string(x);
	} else { // else, print position status
		boardConsoleDisplayHelper::showPositionStatus(board, x, y, perspective);
	}
	return;
}

// print board to console
// may be depreciated if an UI is implemented in the future
void printBoard(Board const &board, int perspective) {
	for (int y = 0; y <= board.height; y++) { // for each row
		boardConsoleDisplayHelper::printRow(board, y);
		for (int x = 1; x <= board.width; x++) { // next to the row label, we print all positions in row
			boardConsoleDisplayHelper::printColumnInRow(board, x, y, perspective);
		}
		std::cout << std::endl;
	}
}

int getValuesWithinRange(std::string const &prompt, int min, int max) { // function to input an int value within range, used for board initialization, probably could be reporopused for other uses
	int ret = min - 1;
	while ((ret < min) || (ret > max)) {
		std::cout << prompt << std::endl;
		std::cout << "value must be between " << min << " and " << max << std::endl;
		std::cin >> ret;
	}
	return ret;
}