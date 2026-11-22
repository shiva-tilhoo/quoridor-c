#include "interaction.h"
#include "display.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h> // Needed for _getch()

void move_pawn(t_game *game) {
    char key;
    t_player *current = &game->players[game->current_player];

    printf("%s's turn (%c).\n", current->name, current->token);
    printf("Move with Z/W (up), S (down), Q/A (left), D (right). Press Esc to quit.\n");

    while (1) {
        key = toupper(_getch());
        t_position new_position = current->position;
        int valid_move = 1;

        if (key == 'S') {
            // Down
            if (new_position.y + 2 < BOARD_SIZE && game->board[new_position.y + 1][new_position.x] != WALL) {
                new_position.y += 2;
            } else {
                printf("Error: move not possible (wall in the way).\n");
                valid_move = 0;
            }
        } else if (key == 'Z' || key == 'W') {
            // Up
            if (new_position.y - 2 >= 0 && game->board[new_position.y - 1][new_position.x] != WALL) {
                new_position.y -= 2;
            } else {
                printf("Error: move not possible (wall in the way).\n");
                valid_move = 0;
            }
        } else if (key == 'D') {
            // Right
            if (new_position.x + 2 < BOARD_SIZE && game->board[new_position.y][new_position.x + 1] != WALL) {
                new_position.x += 2;
            } else {
                printf("Error: move not possible (wall in the way).\n");
                valid_move = 0;
            }
        } else if (key == 'Q' || key == 'A') {
            // Left
            if (new_position.x - 2 >= 0 && game->board[new_position.y][new_position.x - 1] != WALL) {
                new_position.x -= 2;
            } else {
                printf("Error: move not possible (wall in the way).\n");
                valid_move = 0;
            }
        } else if (key == 27) {
            printf("Game over.\n");
            exit(0);
        } else {
            printf("Invalid key.\n");
            continue;
        }

        if (valid_move &&
            new_position.x >= 0 && new_position.x < BOARD_SIZE &&
            new_position.y >= 0 && new_position.y < BOARD_SIZE &&
            game->board[new_position.y][new_position.x] == SQUARE) {
            current->position = new_position;
            current->moves++;

            // On a win the turn does not pass: game_loop() ends the game
            if (!check_victory(game)) {
                game->current_player = (game->current_player + 1) % game->player_count; // Next player
            }
            break;
        } else {
            printf("Invalid move. Try again.\n");
        }
    }
}

bool place_wall(char board[BOARD_SIZE][BOARD_SIZE], t_wall *wall, t_game *game) {
    // Ask for the coordinates and the side
    printf("Where do you want to place the wall? (e.g. D1 E1 B): ");
    if (scanf(" %c%d %c%d %c", &wall->col1, &wall->row1, &wall->col2, &wall->row2,
              &wall->side) != 5) {
        clear_input_buffer();
        printf("Error: invalid input.\n");
        return false;
    }

    // Convert the coordinates (e.g. D1 -> column = 6, row = 0)
    int col1 = (toupper(wall->col1) - 'A') * 2; // Upper case to avoid mistakes
    int col2 = (toupper(wall->col2) - 'A') * 2;
    int row1 = (wall->row1 - 1) * 2;
    int row2 = (wall->row2 - 1) * 2;

    // Check the board limits
    if (col1 < 0 || col1 >= BOARD_SIZE || row1 < 0 || row1 >= BOARD_SIZE ||
        col2 < 0 || col2 >= BOARD_SIZE || row2 < 0 || row2 >= BOARD_SIZE) {
        printf("Error: wall outside the board.\n");
        return false;
    }

    // Check that the squares are neighbours
    if (!(abs(col1 - col2) == 2 && row1 == row2) && !(abs(row1 - row2) == 2 && col1 == col2)) {
        printf("Error: the squares must be neighbours, aligned horizontally or vertically.\n");
        return false;
    }

    // Check the walls left for the player
    if (game->players[game->current_player].walls_left <= 0) {
        printf("Error: you have no walls left!\n");
        return false;
    }

    // Check whether the spot is already taken by another wall
    if (board[row1][col1] == WALL || board[row2][col2] == WALL) {
        printf("Error: there is already a wall at this position.\n");
        return false;
    }

    // Place the wall
    char side = toupper(wall->side);
    if (side == 'T') {
        if (row1 > 0 && board[row1 - 1][col1] == ' ' && board[row2 - 1][col2] == ' ') {
            board[row1 - 1][col1] = WALL;
            board[row2 - 1][col2] = WALL;
        } else {
            printf("Error: cannot place the wall on top.\n");
            return false;
        }
    } else if (side == 'B') {
        if (row1 < BOARD_SIZE - 1 && board[row1 + 1][col1] == ' ' && board[row2 + 1][col2] == ' ') {
            board[row1 + 1][col1] = WALL;
            board[row2 + 1][col2] = WALL;
        } else {
            printf("Error: cannot place the wall at the bottom.\n");
            return false;
        }
    } else if (side == 'L') {
        if (col1 > 0 && board[row1][col1 - 1] == ' ' && board[row2][col2 - 1] == ' ') {
            board[row1][col1 - 1] = WALL;
            board[row2][col2 - 1] = WALL;
        } else {
            printf("Error: cannot place the wall on the left.\n");
            return false;
        }
    } else if (side == 'R') {
        if (col1 < BOARD_SIZE - 1 && board[row1][col1 + 1] == ' ' && board[row2][col2 + 1] == ' ') {
            board[row1][col1 + 1] = WALL;
            board[row2][col2 + 1] = WALL;
        } else {
            printf("Error: cannot place the wall on the right.\n");
            return false;
        }
    } else {
        printf("Error: invalid side.\n");
        return false;
    }

    // Update the walls left for the player
    game->players[game->current_player].walls_left--;
    printf("Wall placed by %s.\n", game->players[game->current_player].name);
    game->current_player = (game->current_player + 1) % game->player_count;
    return true;
}

void game_loop(t_game *game) {
    int won = 0;

    while (!won) {
        display_board(game->board, game->players, game);
        printf("%s's turn (%c).\n", game->players[game->current_player].name,
               game->players[game->current_player].token);

        printf("Available actions:\n");
        printf("1 - Move the pawn\n");
        printf("2 - Place a wall\n");
        printf("3 - Skip the turn\n");
        printf("4 - Quit\n");
        printf("Your choice: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("Invalid choice. Please try again.\n");
            continue;
        }

        switch (choice) {
            case 1:
                move_pawn(game);
                break;

            case 2: {
                t_wall wall;
                if (!place_wall(game->board, &wall, game)) {
                    printf("Invalid action: the wall cannot be placed.\n");
                }
                break;
            }

            case 3:
                printf("%s skips their turn.\n", game->players[game->current_player].name);
                game->current_player = (game->current_player + 1) % game->player_count;
                break;

            case 4: {
                printf("Save the game before quitting? (Y/N): ");
                char answer;
                scanf(" %c", &answer);
                if (toupper(answer) == 'Y') {
                    save_game(game);
                    printf("Game saved. See you soon!\n");
                    wait_for_key();
                    return;
                }
                if (toupper(answer) == 'N') {
                    printf("Game not saved. See you soon!\n");
                    wait_for_key();
                    return;
                }
                printf("Invalid choice. Please try again.\n");
                break;
            }

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

        // Check for a win after each action
        if (check_victory(game)) {
            won = 1;
            game->players[game->current_player].score += WIN_POINTS;
            system("cls");
            printf("Well done %s! You reached the opposite side!\n", game->players[game->current_player].name);

            save_scores(game, SCORES_FILE);
            save_history(game, HISTORY_FILE);

            display_player_summary(game);
            wait_for_key();
        }
    }
}

void start_new_game(t_game *game) {
    memset(game, 0, sizeof(t_game));
    init_board(game->board);
    init_players(game->players, game);
    init_player_positions(game->players, game->player_count);
    load_scores(game, SCORES_FILE);
    game->current_player = random_int(game->player_count); // The first player is drawn at random
    system("cls");
    game_loop(game);
}

void resume_game(t_game *game) {
    system("cls");
    printf("Resuming the game.\n");
    game_loop(game);
}
