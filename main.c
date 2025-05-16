#include "include/Player.h"
#include "include/TicTacToe.h"

// Player 1 Character
#define PLAYER_1_CHAR 'X'

// Player 2 Character
#define PLAYER_2_CHAR 'O'

// Length of the side of the board 
// 3 - 3x3, 4 - 4x4, etc
#define BOARD_SIDE_LENGTH 3

int main() {
	Player player_1 = player_create('X'), player_2 = player_create('O');
	Player *players[2] = { &player_1, &player_2 };

	TicTacToe game = game_create(3, players);

	game_start(&game);

	return 0;
}
