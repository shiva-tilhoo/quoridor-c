#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define BOARD_SIZE 17

#define BOARD_SAVE_FILE "saved_board.txt"
#define PLAYERS_SAVE_FILE "saved_players.txt"
#define SCORES_FILE "scores.txt"
#define HISTORY_FILE "history.txt"

#define WIN_POINTS 5

#define SQUARE '.'
#define WALL '#'

typedef struct {
    int x;
    int y;
} t_position;

typedef struct {
    char name[40];
    char token; // Player symbol (e.g. 'A', 'B')
    t_position position; // Current position of the player
    int walls_left; // Number of walls left
    int score; // Score of the games won
    int moves; // Number of moves made
} t_player;

typedef struct {
    char board[BOARD_SIZE][BOARD_SIZE];
    t_player players[4]; // Four players at most
    int current_player; // Index of the player whose turn it is (0 to 3)
    int player_count; // Number of players (2 or 4)
} t_game;

typedef struct {
    char col1;
    char col2;
    int row1;
    int row2;
    char side;
} t_wall;

// Initialisation
void init_board(char board[BOARD_SIZE][BOARD_SIZE]);
void init_walls(t_player *players, int player_count);
void init_players(t_player *players, t_game *game);
void init_player_positions(t_player players[], int player_count);

// Rules
bool check_victory(t_game *game);

// Saving / loading a game
void save_board(t_game *game);
void save_players(t_game *game);
void save_game(t_game *game);
bool load_board(t_game *game);
bool load_players(t_game *game);
bool load_game(t_game *game);

// Scores and history
void load_scores(t_game *game, const char *file_name);
void save_scores(t_game *game, const char *file_name);
void save_history(t_game *game, const char *history_file);

#endif
