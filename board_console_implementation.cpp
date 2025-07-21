#include "board_console_implementation.h"

const int undefinedPerspective = -1;

namespace printBoardAuxiliars {
	void showPositionStatus(Board const &board, unsigned int x, unsigned int y, int perspective = undefinedPerspective);

	void printRow(Board const &board, unsigned int y);

	void printColumnInRow(Board const &board, unsigned int x, unsigned int y, int perspective);
}

void printBoardAuxiliars::showPositionStatus(Board const &board, unsigned int x, unsigned int y, int perspective) {
	// print the status of the position {x, y}
	//(" " = no existe, "O" = posicion en juego/estado desconocido, "M" = mina del jugador en la posicion)
	bool isPositionEnabled = true;

	for (int i = 0; i < board.disabledPositions.size(); i++) {
		Position disabledPos = board.disabledPositions[i];
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

void printBoardAuxiliars::printRow(Board const &board, unsigned int y) {
	int spaceLength = (3 - std::to_string(y).length());
	if (y == 0) { // si es la primera fila, imprimimos solo espacio
		std::cout << "   ";
	} else { // caso contrario, imprimimos el numero de la fila
		std::cout << y;
		for (int space = 0; space < spaceLength; space++) {
			std::cout << ' ';
		};
	}
}

void printBoardAuxiliars::printColumnInRow(Board const &board, unsigned int x, unsigned int y, int perspective) {
	int spaceLength = (3 - std::to_string(x).length());
	if (y == 0) { // si es la primera fila, imprimimos los numeros de las columnas
		for (int space = 0; space < spaceLength; space++) {
			std::cout << ' ';
		};
		std::cout << std::to_string(x);
	} else { // en caso contrario, imprimimos el estado de la posicion
		printBoardAuxiliars::showPositionStatus(board, x, y, perspective);
	}
	return;
}

// print board to console
// may be depreciated if an UI is implemented in the future
void printBoard(Board const &board, int perspective) {
	for (int y = 0; y <= board.height; y++) { // para cada fila
		printBoardAuxiliars::printRow(board, y);
		for (int x = 1; x <= board.width; x++) { // seguido del numero de fila, imprimimos todas las posiciones de la fila
			printBoardAuxiliars::printColumnInRow(board, x, y, perspective);
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