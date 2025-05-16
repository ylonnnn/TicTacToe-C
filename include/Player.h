#include <stdbool.h>

typedef struct TicTacToe TicTacToe;

typedef struct Player {
	char mark;
	
	void (*play)(struct Player *player, TicTacToe *game);
} Player;

Player player_create(char mark);

void player_play(Player *player, TicTacToe *game);

int player_get_turn(Player *player, TicTacToe *game);

bool player_place_mark(Player *player, TicTacToe *game, int slot);
