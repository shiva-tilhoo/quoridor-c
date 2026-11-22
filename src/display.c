#include "display.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // Needed for _getch()

void display_board(char board[BOARD_SIZE][BOARD_SIZE], t_player players[], t_game *game) {
    printf("   Current player: %s  | Token: %c  | Walls left: %d\n\n\n",
           players[game->current_player].name,
           players[game->current_player].token,
           players[game->current_player].walls_left);

    printf("    ");
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (i % 2 == 0) {
            printf("%c ", 'A' + i / 2); // Column letters
        } else {
            printf("    ");
        }
    }
    printf("\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        if (i % 2 == 0) {
            printf("%2d ", (i / 2) + 1); // Row numbers
        } else {
            printf("   ");
        }

        for (int j = 0; j < BOARD_SIZE; j++) {
            int player_here = 0;
            for (int k = 0; k < game->player_count; k++) {
                if (i == players[k].position.y && j == players[k].position.x) {
                    set_color(4 + k, 15); // One colour per player
                    printf(" %c ", players[k].token);
                    set_color(15, 0);
                    player_here = 1;
                    break;
                }
            }
            if (!player_here) {
                set_color(15, 0);
                printf(" %c ", board[i][j]);
            }
        }
        printf("\n");
    }
}

void display_player_summary(t_game *game) {
    printf("\nPlayer summary:\n");
    for (int i = 0; i < game->player_count; i++) {
        printf("Player: %s\n", game->players[i].name);
        printf("  - Walls left: %d\n", game->players[i].walls_left);
        printf("  - Score: %d\n", game->players[i].score);
        printf("  - Moves made: %d\n", game->players[i].moves);
    }
    printf("\n");
}

void display_menu(int selected_option) {
    system("cls"); // Clear the screen (Windows)
    printf("\n===== QUORIDOR MENU =====\n");
    printf("\n Use the UP/DOWN arrows to navigate and ENTER to select.\n\n");

    char *options[] = {
        "1. Start a new game",
        "2. Resume a saved game",
        "3. Show help",
        "4. Show player scores",
        "5. Quit"
    };
    int option_count = sizeof(options) / sizeof(options[0]);

    for (int i = 0; i < option_count; i++) {
        if (i == selected_option) {
            printf(" > %s <\n", options[i]); // Highlighted option
        } else {
            printf("   %s\n", options[i]);
        }
    }
}

void display_help() {
    system("cls");
    printf("\n===== HELP =====\n");
    printf("\nQuoridor is played on a 9x9 board.\n");
    printf("Each player must reach the side opposite their starting position.\n");
    printf("On your turn, move your pawn or place a wall to slow down your opponents.\n");
    printf("\nPress any key to return to the menu...");
    _getch();
}

void display_scores(const char *file_name) {
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("No scores recorded yet.\n");
        printf("\nPress any key to return to the menu...");
        _getch();
        return;
    }

    system("cls");
    printf("\n===== PLAYER SCORES =====\n");
    char name[40];
    int score;
    while (fscanf(fp, "%39s %d", name, &score) == 2) {
        printf("Player: %s | Score: %d\n", name, score);
    }

    fclose(fp);
    printf("\nPress any key to return to the menu...");
    _getch();
}
