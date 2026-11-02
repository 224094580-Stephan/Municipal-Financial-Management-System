/* utilities.h - reusable input/output helper functions */
#ifndef UTILITIES_H
#define UTILITIES_H

#include <stddef.h>

/* Print a boxed title, e.g. a menu or report heading. */
void displayHeader(const char *title);

/* Prompt until the user types a non-empty line (stored without '\n'). */
void readString(const char *prompt, char *buffer, size_t size);

/* Prompt until the user types a whole number between min and max. */
int readInt(const char *prompt, int min, int max);

/* Prompt until the user types a number that is >= min. */
double readDouble(const char *prompt, double min);

/* Wait for the user to press Enter. */
void pauseScreen(void);

#endif
