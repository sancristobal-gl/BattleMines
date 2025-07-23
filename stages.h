#include "game_logic.h"

namespace gameStages {
	void roundStart(Board const &board);

	int minePlacement(Board &board);

	int guessing(Board &board);

	void roundEnd(Board &board);
}
