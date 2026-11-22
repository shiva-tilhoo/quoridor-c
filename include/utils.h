#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// Change the text colour in the console
void set_color(int text_color, int background_color);

// Random number between 0 and max (excluded)
int random_int(int max);

// Pause until the user presses a key
void wait_for_key();

// Clear the input buffer
void clear_input_buffer();

// Check whether a string is blank
bool is_blank(const char *text);

#endif // UTILS_H
