#ifndef DISPLAY_H
#define DISPLAY_H

#include "game.h"

void display_board(char board[BOARD_SIZE][BOARD_SIZE], t_player players[], t_game *game);
void display_player_summary(t_game *game);
void display_menu(int selected_option);
void display_help();
void display_scores(const char *file_name);

#endif
