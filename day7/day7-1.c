#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#define BUFSIZE 256

#define ARRSIZE 1024
#define HNDSIZE 5

enum HandType {HIGH, PAIR, PAIR2, THREE, FULL, FOUR, FIVE};

struct hand
{
    char card[HNDSIZE];
    enum HandType type;
};

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

    // PROCESS
    while (!feof(file)) {
        // GET ROW
        fgets(buffer, BUFSIZE, file);
        if (!*buffer) break;
        cur = buffer;
        // READING
        // PROCESSING
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