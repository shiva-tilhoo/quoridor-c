#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h> // For _getch()
#include "utils.h"

void set_color(int text_color, int background_color) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, background_color * 16 + text_color);
}

int random_int(int max) {
    if (max <= 0) {
        return 0; // Return 0 when max is invalid
    }
    return rand() % max;
}

void wait_for_key() {
    printf("Press any key to continue...");
    _getch(); // Wait for a key without echoing it
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Drain the input buffer
    }
}

bool is_blank(const char *text) {
    if (text == NULL || text[0] == '\0') {
        return true; // Null or empty string
    }
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] != ' ' && text[i] != '\t') {
            return false; // Contains at least one non-blank character
        }
    }
    return true; // The string only contains spaces or tabs
}
