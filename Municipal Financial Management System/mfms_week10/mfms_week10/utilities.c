/* utilities.c - reusable input/output helper functions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include "utilities.h"

#define LINE_SIZE 256

/* Read one line from stdin without the newline.
   Returns 1 on success, 0 on end of input. Over-long lines are discarded. */
static int readLine(char *buffer, size_t size)
{
    size_t length;
    int ch;

    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }

    length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    } else {
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }
    return 1;
}

static void endOfInput(void)
{
    printf("\nInput closed. Exiting MFMS.\n");
    exit(EXIT_SUCCESS);
}

static int isBlank(const char *text)
{
    while (*text != '\0') {
        if (!isspace((unsigned char)*text)) {
            return 0;
        }
        text++;
    }
    return 1;
}

void displayHeader(const char *title)
{
    printf("\n===============================================\n");
    printf("  %s\n", title);
    printf("===============================================\n");
}

void readString(const char *prompt, char *buffer, size_t size)
{
    for (;;) {
        printf("%s", prompt);
        if (!readLine(buffer, size)) {
            endOfInput();
        }
        if (isBlank(buffer)) {
            printf("Input cannot be empty. Please try again.\n");
            continue;
        }
        /* '|' separates fields in the saved text files, so it cannot be
           allowed inside a field. */
        if (strchr(buffer, '|') != NULL) {
            printf("The '|' character is not allowed. Please try again.\n");
            continue;
        }
        return;
    }
}

int readInt(const char *prompt, int min, int max)
{
    char line[LINE_SIZE];
    char *end;
    long value;

    for (;;) {
        printf("%s", prompt);
        if (!readLine(line, sizeof line)) {
            endOfInput();
        }

        errno = 0;
        value = strtol(line, &end, 10);

        if (end == line || errno == ERANGE) {
            printf("Invalid input. Please enter a whole number.\n");
            continue;
        }
        if (!isBlank(end)) {
            printf("Invalid input. Please enter a whole number.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Please enter a number between %d and %d.\n", min, max);
            continue;
        }
        return (int)value;
    }
}

double readDouble(const char *prompt, double min)
{
    char line[LINE_SIZE];
    char *end;
    double value;

    for (;;) {
        printf("%s", prompt);
        if (!readLine(line, sizeof line)) {
            endOfInput();
        }

        errno = 0;
        value = strtod(line, &end);

        if (end == line || errno == ERANGE || !isfinite(value) || !isBlank(end)) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < min) {
            printf("Value must be at least %.2f.\n", min);
            continue;
        }
        return value;
    }
}

void pauseScreen(void)
{
    char line[LINE_SIZE];

    printf("\nPress Enter to continue...");
    readLine(line, sizeof line);
}
