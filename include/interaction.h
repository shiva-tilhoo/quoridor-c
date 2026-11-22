#ifndef INTERACTION_H
#define INTERACTION_H

#include "game.h"

void move_pawn(t_game *game);
bool place_wall(char board[BOARD_SIZE][BOARD_SIZE], t_wall *wall, t_game *game);
void game_loop(t_game *game);
void start_new_game(t_game *game);
void resume_game(t_game *game);

#endif
