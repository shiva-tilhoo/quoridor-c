#include "game.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_board(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (i % 2 == 0 && j % 2 == 0) {
                board[i][j] = SQUARE; // Playable squares
            } else {
                board[i][j] = ' '; // Gaps where walls can be placed
            }
        }
    }
}

void init_walls(t_player *players, int player_count) {
    int walls_per_player;

    if (player_count == 2) {
        walls_per_player = 10; // Each player gets 10 walls
    } else if (player_count == 4) {
        walls_per_player = 5; // Each player gets 5 walls
    } else {
        printf("Error: invalid number of players. Only 2 or 4 players are allowed.\n");
        return;
    }

    for (int i = 0; i < player_count; i++) {
        players[i].walls_left = walls_per_player;
    }
}

void init_players(t_player *players, t_game *game) {
    do {
        set_color(3, 0);
        printf("Enter the number of players (2 or 4): ");
        if (scanf("%d", &game->player_count) != 1) {
            printf("Error: invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }
        while (getchar() != '\n'); // Clear what is left in the buffer
    } while (game->player_count != 2 && game->player_count != 4);

    for (int i = 0; i < game->player_count; i++) {
        int valid_input = 0; // Tracks whether the input is valid
        do {
            printf("Enter the name of player %d: ", i + 1);

            if (scanf("%39s", players[i].name) == 1) {
                // A word was read correctly
                valid_input = 1;
            } else {
                printf("Error: invalid input. Please enter a name.\n");
                while (getchar() != '\n'); // Clear the input buffer
            }
        } while (!valid_input); // Repeat until the input is valid

        while (getchar() != '\n'); // Clear what is left in the buffer

        printf("Enter their token (single character): ");
        if (scanf(" %c", &players[i].token) != 1) {
            printf("Error: invalid input for the token.\n");
            while (getchar() != '\n'); // Clear the input buffer
            i--; // Ask again for this player
            continue;
        }
        while (getchar() != '\n'); // Clear the input buffer
        players[i].score = 0;
        players[i].moves = 0;
        init_walls(game->players, game->player_count);
    }
}

void init_player_positions(t_player players[], int player_count) {
    if (player_count == 2) {
        players[0].position.x = BOARD_SIZE / 2;
        players[0].position.y = 0;

        players[1].position.x = BOARD_SIZE / 2;
        players[1].position.y = BOARD_SIZE - 1;
    } else if (player_count == 4) {
        players[0].position.x = BOARD_SIZE / 2;
        players[0].position.y = 0;

        players[1].position.x = BOARD_SIZE / 2;
        players[1].position.y = BOARD_SIZE - 1;

        players[2].position.x = 0;
        players[2].position.y = BOARD_SIZE / 2;

        players[3].position.x = BOARD_SIZE - 1;
        players[3].position.y = BOARD_SIZE / 2;
    }
}

// Checks whether the current player has reached the side opposite their starting point
bool check_victory(t_game *game) {
    t_player *player = &game->players[game->current_player];

    if (game->current_player == 0 && player->position.y == BOARD_SIZE - 1) {
        return true;
    } else if (game->current_player == 1 && player->position.y == 0) {
        return true;
    } else if (game->player_count == 4) {
        if (game->current_player == 2 && player->position.x == BOARD_SIZE - 1) {
            return true;
        } else if (game->current_player == 3 && player->position.x == 0) {
            return true;
        }
    }
    return false;
}

void save_board(t_game *game) {
    FILE *file = fopen(BOARD_SAVE_FILE, "w");
    if (file == NULL) {
        printf("Error: cannot open the save file.\n");
        return;
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            fprintf(file, "%c", game->board[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

bool load_board(t_game *game) {
    int c;
    FILE *file = fopen(BOARD_SAVE_FILE, "r");
    if (file == NULL) {
        printf("Error: cannot open the save file.\n");
        return false;
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            c = fgetc(file);
            if (c == EOF) {
                printf("File error");
                fclose(file);
                return false;
            }
            game->board[i][j] = (char) c;
        }
        fgetc(file); // Line break at the end of the row
    }
    fclose(file);
    return true;
}

void save_players(t_game *game) {
    FILE *file = fopen(PLAYERS_SAVE_FILE, "w");
    if (file == NULL) {
        printf("Error: cannot open the save file.\n");
        return;
    }

    fprintf(file, "%d %d\n", game->player_count, game->current_player);
    for (int i = 0; i < game->player_count; i++) {
        fprintf(file, "%s %c %d %d %d %d %d\n",
                game->players[i].name,
                game->players[i].token,
                game->players[i].walls_left,
                game->players[i].score,
                game->players[i].position.x,
                game->players[i].position.y,
                game->players[i].moves);
    }
    fclose(file);
    file = NULL;
    printf("File saved successfully.\n");
}

bool load_players(t_game *game) {
    FILE *file = fopen(PLAYERS_SAVE_FILE, "r");
    if (file == NULL) {
        printf("Error: cannot open the save file.\n");
        return false;
    }

    int player_count = 0;
    int current_player = 0;
    if (fscanf(file, "%d %d\n", &player_count, &current_player) != 2 ||
        (player_count != 2 && player_count != 4) ||
        current_player < 0 || current_player >= player_count) {
        printf("Error: invalid players file.\n");
        fclose(file);
        return false;
    }

    int read_count = 0;
    while (read_count < player_count &&
           fscanf(file, "%39s %c %d %d %d %d %d\n",
                  game->players[read_count].name,
                  &game->players[read_count].token,
                  &game->players[read_count].walls_left,
                  &game->players[read_count].score,
                  &game->players[read_count].position.x,
                  &game->players[read_count].position.y,
                  &game->players[read_count].moves) == 7) {
        // Check that the player name is valid (not empty)
        if (strlen(game->players[read_count].name) == 0) {
            printf("Error: empty player name found. Stopping.\n");
            break;
        }
        read_count++;
    }
    fclose(file);

    if (read_count != player_count) {
        printf("Error: incomplete players file.\n");
        return false;
    }
    game->player_count = player_count;
    game->current_player = current_player;
    return true;
}

void save_game(t_game *game) {
    save_board(game);
    save_players(game);
}

bool load_game(t_game *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->board[i][j] = ' ';
        }
    }
    game->player_count = 0;
    if (!load_board(game) || !load_players(game)) {
        game->player_count = 0;
        return false;
    }
    printf("Game loaded successfully\n");
    return true;
}

void save_scores(t_game *game, const char *file_name) {
    // Load the existing scores into a temporary structure
    char known_names[100][40];
    int known_scores[100];
    int known_count = 0;

    FILE *fp = fopen(file_name, "r");
    if (fp != NULL) {
        while (known_count < 100 &&
               fscanf(fp, "%39s %d", known_names[known_count],
                      &known_scores[known_count]) == 2) {
            known_count++;
        }
        fclose(fp);
    }

    // Update the scores of the current players
    for (int i = 0; i < game->player_count; i++) {
        int found = 0;
        for (int j = 0; j < known_count; j++) {
            if (strcmp(game->players[i].name, known_names[j]) == 0) {
                known_scores[j] = game->players[i].score; // Update the score
                found = 1;
                break;
            }
        }
        if (!found && known_count < 100) {
            // Add a new player when not found
            strcpy(known_names[known_count], game->players[i].name);
            known_scores[known_count] = game->players[i].score;
            known_count++;
        }
    }

    // Rewrite the whole file
    fp = fopen(file_name, "w");
    if (fp == NULL) {
        printf("Error: cannot open the save file.\n");
        return;
    }
    for (int i = 0; i < known_count; i++) {
        fprintf(fp, "%s %d\n", known_names[i], known_scores[i]);
    }

    fclose(fp);
    printf("Scores saved to %s.\n", file_name);
}

void load_scores(t_game *game, const char *file_name) {
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("No score file found. Scores start at zero.\n");
        return;
    }

    char name[40];
    int score;
    while (fscanf(fp, "%39s %d", name, &score) == 2) {
        for (int i = 0; i < game->player_count; i++) {
            if (strcmp(name, game->players[i].name) == 0) {
                game->players[i].score = score; // Restore the existing score
                break;
            }
        }
    }

    fclose(fp);
    printf("Scores loaded from %s.\n", file_name);
}

void save_history(t_game *game, const char *history_file) {
    FILE *fp = fopen(history_file, "a");
    if (fp == NULL) {
        printf("Error: cannot open the history file.\n");
        return;
    }

    fprintf(fp, "New game:\n");
    for (int i = 0; i < game->player_count; i++) {
        fprintf(fp, "Player: %s, Score: %d, Walls left: %d, Moves: %d\n",
                game->players[i].name,
                game->players[i].score,
                game->players[i].walls_left,
                game->players[i].moves);
    }
    fprintf(fp, "Final board:\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            fprintf(fp, "%c", game->board[i][j]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "-----------------------------\n");

    fclose(fp);
    printf("History saved successfully.\n");
}
