#pragma once

#include <stdbool.h>

#include "dynamic_array.h"
#include "types.h"

typedef struct Player Player;

typedef struct TicTacToe {
	int size;
	Cell **board;

	int turn;
	Player *players[2];
	Player *winner;

    int available_moves;
} TicTacToe;

TicTacToe game_create(int size, Player *players[2]);

void game_initialize_board(TicTacToe *game);
void game_map_board(TicTacToe *game);

void game_initialize_turn(TicTacToe *game);
int game_get_turn(TicTacToe *game, Player *player);

bool game_exceeds_board(TicTacToe *game, int slot);
Cell *game_get_cell(TicTacToe *game, int slot);
array game_get_empty_cells(TicTacToe *game);

bool game_has_mark(TicTacToe *game, int slot);

void game_check(TicTacToe *game, int turn);

int game_required_moves(TicTacToe *game, array *orientation, int turn);

array game_get_optimal_orientation(TicTacToe *game, int turn);
array game_get_horizontal_orientations(TicTacToe *game, int turn);
array game_get_vertical_orientations(TicTacToe *game, int turn);
array game_get_diagonal_orientations(TicTacToe *game, int turn);

void game_start(TicTacToe *game);
void game_process(TicTacToe *game);
void game_end(TicTacToe *game);
