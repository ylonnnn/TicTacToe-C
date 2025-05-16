#include <stdio.h>
#include <math.h>

#include "../include/Player.h"
#include "../include/TicTacToe.h"

void player_play(Player *player, TicTacToe *game) {
	int turn = player_get_turn(player, game), slot;
	char mark = player->mark;
	
	printf("[%d] Enter the slot to place your mark [%c]: ", turn, mark);
	scanf("%d", &slot);

	int max_cell = pow(game->size, 2);
	if (slot < 1 || slot > max_cell) {
		printf("Slot must at least be %d and be %d at most!", 1, max_cell);

		return player_play(player, game);
	}

	// Mark Placement
	slot--;	
	if (!player_place_mark(player, game, slot)) {
		printf("Invalid slot!");

		return player_play(player, game);
	}

	game_map_board(game);
	game_process(game);
}

Player player_create(char mark) {
	return (Player) {
		.mark = mark,
		.play = player_play,
	};
}

int player_get_turn(Player *player, TicTacToe *game) {
	return game_get_turn(game, player);
}

bool player_place_mark(Player *player, TicTacToe *game, int slot) {
	// Check if slot is within the board
	if (game_exceeds_board(game, slot))
		return false;

	// Check if the cell is empty;
	if (game_has_mark(game, slot))
		return false;

	Cell *cell = game_get_cell(game, slot);
	cell->mark = player->mark;

    game->available_moves--;

	return true;
}
