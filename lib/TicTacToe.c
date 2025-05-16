#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/Player.h"
#include "../include/TicTacToe.h"
#include "../include/utils.h"

TicTacToe game_create(int size, Player *players[2]) {
  TicTacToe game = (TicTacToe){.size = size,
                               .players = {players[0], players[1]},
                               .available_moves = size * size};
  game_initialize_board(&game);

  return game;
}

void game_initialize_board(TicTacToe *game) {
  int size = game->size;

  game->board = malloc(size * sizeof(Cell *));
  if (game->board == NULL) {
    perror("Failed to allocate memory for the grid!");
    return;
  }

  for (int i = 0; i < size; i++) {
    Cell *row = malloc(size * sizeof(Cell));
    if (row == NULL) {
      perror("Failed to allocated memory for row!");
      return;
    }

    game->board[i] = row;

    for (int j = 0; j < size; j++) {
      Cell *cell = &row[j];

      cell->mark = ' ';
      cell->position[0] = i;
      cell->position[1] = j;

      // printf("%c <%d, %d>\t", cell->mark, cell->position[0],
      // cell->position[1]);
    }
  }
}

void game_map_board(TicTacToe *game) {
  int size = game->size, r_len = 3,
      line_len = (r_len + 1) * (size + 1) * sizeof(char) + 1;

  char *h_line = malloc(line_len);
  if (h_line == NULL) {
    perror("Failed to allocate memory to board line!");
  }

  memset(h_line, '-', line_len);

  h_line[line_len - 1] = '\0';

  printf("%s\n", h_line);

  for (int i = 0; i < size; i++) {
    printf("| ");

    for (int j = 0; j < size; j++) {
      printf("%c ", game->board[i][j].mark);

      if (j < (size - 1))
        printf(" | ");
    }

    printf(" |\n%s\n", h_line);
  }

  printf("\n");
}

void game_initialize_turn(TicTacToe *game) { game->turn = random(1, 2); }

int game_get_turn(TicTacToe *game, Player *player) {
  return player == game->players[0] ? 1 : 2;
}

bool game_exceeds_board(TicTacToe *game, int slot) {
  return slot < 0 || slot >= (game->size * game->size);
}

Cell *game_get_cell(TicTacToe *game, int slot) {
  int size = game->size, row = slot / size, col = slot % size;

  return &game->board[row][col];
}

array game_get_empty_cells(TicTacToe *game) {
  int size = game->size, sqr = size * size;

  array empty_cells = array_create(sqr, sizeof(Cell *));
  if (empty_cells.data == NULL)
    return empty_cells;

  for (int i = 0; i < sqr; i++) {
    if (game_exceeds_board(game, i) || game_has_mark(game, i))
      continue;

    array_push_back(&empty_cells, game_get_cell(game, i));
  }

  array_update_cap(&empty_cells, empty_cells.size);

  return empty_cells;
}

bool game_has_mark(TicTacToe *game, int slot) {
  Cell *cell = game_get_cell(game, slot);

  return cell->mark == game->players[0]->mark ||
         cell->mark == game->players[1]->mark;
}

void game_check(TicTacToe *game, int turn) {
  array optimal = game_get_optimal_orientation(game, turn);
  int required = game_required_moves(game, &optimal, turn);

  if (required == 0) {
    game->winner = game->players[turn - 1];
  }
}

int game_required_moves(TicTacToe *game, array *orientation, int turn) {
  int size = game->size, moves = 0;

  if (orientation->size == 0)
    return size;

  char mark = game->players[turn - 1]->mark;

  for (int i = 0; i < size; i++) {
    Cell *cell = *(Cell **)(array_get(orientation, i));

    if (cell->mark == game->players[turn == 1 ? 1 : 0]->mark)
      return size;

    if (cell->mark != mark)
      moves++;
  }

  return moves;
}

void optimal_update_fn(array *orientation, int *required_moves,
                       array *n_orientation, int n_required_moves) {
  *orientation = *n_orientation;
  *required_moves = n_required_moves;
}

void optimal_fn(TicTacToe *game, array *c_orientation, int *required_moves,
                array *orientation, int turn) {
  if (orientation->size == 0)
    return;

  int req_moves = game_required_moves(game, orientation, turn);
  if (req_moves > *required_moves)
    return;

  if (req_moves < *required_moves)
    optimal_update_fn(c_orientation, required_moves, orientation, req_moves);

  if (req_moves == *required_moves && random(0, 100) < 50)
    optimal_update_fn(c_orientation, required_moves, orientation, req_moves);
}

array game_get_optimal_orientation(TicTacToe *game, int turn) {
  int size = game->size;

  array orientation = array_create(size, sizeof(Cell *));
  int req_moves = INT32_MAX;

  array horizontal = game_get_horizontal_orientations(game, turn),
        vertical = game_get_vertical_orientations(game, turn),
        diagonal = game_get_diagonal_orientations(game, turn);

  for (int i = 0; i < size; i++) {
    if (i < horizontal.size) {
      optimal_fn(game, &orientation, &req_moves, array_get(&horizontal, i),
                 turn);
    }

    if (i < vertical.size) {
      optimal_fn(game, &orientation, &req_moves, array_get(&vertical, i), turn);
    }

    if (i < diagonal.size) {
      optimal_fn(game, &orientation, &req_moves, array_get(&diagonal, i), turn);
    }
  }

  for (int i = 0; i < size; i++) {
    if (i < horizontal.size) {
      array *f_orientation = array_get(&horizontal, i);
      if (f_orientation->data != orientation.data) {
        free(f_orientation->data);
      }
    }

    if (i < vertical.size) {
      array *f_orientation = array_get(&vertical, i);
      if (f_orientation->data != orientation.data) {
        free(f_orientation->data);
      }
    }

    if (i < diagonal.size) {
      array *f_orientation = array_get(&diagonal, i);
      if (f_orientation->data != orientation.data) {
        free(f_orientation->data);
      }
    }
  }

  return orientation;
}

array game_get_horizontal_orientations(TicTacToe *game, int turn) {
  char opp_mark = game->players[turn == 1 ? 1 : 0]->mark;
  int size = game->size;

  array orientations = array_create(size, sizeof(array));

  for (int i = 0; i < size; i++) {
    array orientation = array_create(size, sizeof(Cell *));

    for (int j = 0; j < size; j++) {
      Cell *cell = game_get_cell(game, (i * size) + j);

      if (cell->mark == opp_mark)
        break;

      array_push_back(&orientation, &cell);
    }

    if (orientation.size < size) {
      free(orientation.data);
      continue;
    }

    array_push_back(&orientations, &orientation);
  }

  return orientations;
}

array game_get_vertical_orientations(TicTacToe *game, int turn) {
  char opp_mark = game->players[turn == 1 ? 1 : 0]->mark;
  int size = game->size;

  array orientations = array_create(size, sizeof(array));

  for (int i = 0; i < size; i++) {
    array orientation = array_create(size, sizeof(Cell *));

    for (int j = 0; j < size; j++) {
      Cell *cell = game_get_cell(game, (j * size) + i);

      if (cell->mark == opp_mark)
        break;

      array_push_back(&orientation, &cell);
    }

    if (orientation.size < size) {
      free(orientation.data);
      continue;
    }

    array_push_back(&orientations, &orientation);
  }

  return orientations;
}

array game_get_diagonal_orientations(TicTacToe *game, int turn) {
  char opp_mark = game->players[turn == 1 ? 1 : 0]->mark;
  int or_c = 2, size = game->size;

  array orientations = array_create(or_c, sizeof(array));
  bool valid[2] = {true, true};

  array dg_orientation = array_create(size, sizeof(Cell *)),
        adg_orientation = array_create(size, sizeof(Cell *));

  for (int i = 0; i < size; i++) {
    // Diagonal [i][i]
    {

      if (valid[0]) {
        Cell *cell = game_get_cell(game, (i * size) + i);

        if (cell->mark == opp_mark)
          valid[0] = false;

        array_push_back(&dg_orientation, &cell);
      }
    }

    // Anti-Diagonal [i][size - (i + 1)]
    {
      if (valid[1]) {
        Cell *cell = game_get_cell(game, (i * size) + (size - (i + 1)));

        if (cell->mark == opp_mark)
          valid[1] = false;

        array_push_back(&adg_orientation, &cell);
      }
    }
  }

  if (valid[0])
    array_push_back(&orientations, &dg_orientation);

  if (valid[1])
    array_push_back(&orientations, &adg_orientation);

  return orientations;
}

void game_start(TicTacToe *game) {
  game_initialize_board(game);
  game_initialize_turn(game);

  game_map_board(game);

  Player *player = game->players[game->turn - 1];
  player->play(player, game);
}

void game_process(TicTacToe *game) {
  game_check(game, game->turn);

  if (game->winner != NULL || game->available_moves <= 0) {
    game_end(game);

    return;
  }

  game->turn = game->turn == 1 ? 2 : 1;
  Player *player = game->players[game->turn - 1];

  player->play(player, game);
}

void game_end(TicTacToe *game) {
  if (game->winner == NULL) {
    printf("\nDRAW\n");
  } else {
    printf("WINNER: Player %d [%c]", game->turn, game->winner->mark);
  }

  for (int i = 0; i < game->size; i++) {
    free(game->board[i]);
  }

  free(game->board);
}
