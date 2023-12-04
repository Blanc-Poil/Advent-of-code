#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#define BUFSIZE 1024

#define ARRSIZE 64

unsigned scan_unsigned(char** cur);

int main(int argc, char* argv[])
{
    //no file exception
    if (argc < 2) {
        printf("[NO FILE GIVEN !]\n");
        return 0;
    }

    // INIT
    FILE* file = fopen(argv[1], "r");
    char buffer[BUFSIZE];
    char* cur = NULL;
    unsigned sum = 0;
    // context
    unsigned winnings[ARRSIZE];
    unsigned* win = NULL;
    unsigned* win_end = NULL;
    unsigned havings[ARRSIZE];
    unsigned* have = NULL;
    unsigned* have_end = NULL;

    // PROCESS
    while (!feof(file)) {
        // GET ROW
        fgets(buffer, BUFSIZE, file);
        if (!*buffer) break;
        cur = buffer;
        // jump
        while (*cur != ':') cur++;
        do cur++; while (*cur == ' ');
        // READING
        // get winnings
        win = winnings;
        while (*cur != '|') {
            *win = scan_unsigned(&cur);
            win++;
            // jump
            while (*cur == ' ') cur++;
        }
        win_end = win;
        // jump
        do cur++; while (*cur == ' ');
        // get havings
        have = havings;
        while (*cur != '\n') {
            *have = scan_unsigned(&cur);
            have++;
            // jump
            while (*cur == ' ') cur++;
        }
        have_end = have;
        // PROCESSING
        unsigned points = 0;
        win = winnings;
        while (win != win_end) {
            have = havings;
            while (have != have_end) {
                if (*have == *win) {
                    points = (points) ? points*2 : 1;
                    break;
                }
                have++;
            }
            win++;
        }
        sum += points;
        // RESET BUFFER
        *buffer = '\0';
    }

    // RESULT
    printf("Sum: %u\n", sum);

    // return
    return 0;
}

unsigned scan_unsigned(char** cur)
{
    unsigned value = 0;
    while (**cur >= '0' && **cur <= '9') {
        value *= 10;
        value += **cur - '0';
        (*cur)++;
    }
    return value;
}