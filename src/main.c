#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h> // For _getch()
#include "game.h"
#include "interaction.h"
#include "display.h"
#include "utils.h"

#define UP_ARROW 72
#define DOWN_ARROW 80
#define ENTER 13

int main() {
    int selected_option = 0;
    int option_count = 5;
    int running = 1;

    srand((unsigned int) time(NULL));

    t_game *game = (t_game *) malloc(sizeof(t_game));
    if (!game) {
        printf("Memory allocation error for the game.\n");
        return 1;
    }
    memset(game, 0, sizeof(t_game)); // Initialise the structure

    while (running) {
        display_menu(selected_option);
        char key = _getch();

        switch (key) {
            case UP_ARROW:
                selected_option = (selected_option - 1 + option_count) % option_count;
                break;

            case DOWN_ARROW:
                selected_option = (selected_option + 1) % option_count;
                break;

            case ENTER:
                system("cls");
                switch (selected_option) {
                    case 0: // New game
                        start_new_game(game);
                        break;

                    case 1: // Resume a saved game
                        if (load_game(game)) {
                            resume_game(game);
                        } else {
                            printf("Cannot resume: no saved game.\n");
                            wait_for_key();
                        }
                        break;

                    case 2: // Help
                        display_help();
                        break;

                    case 3: // Scores
                        display_scores(SCORES_FILE);
                        break;

                    case 4: // Quit
                        printf("Quitting. Thanks for playing!\n");
                        running = 0;
                        break;
                }
                break;
        }
    }

    free(game);
    return 0;
}
