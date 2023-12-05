#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#define BUFSIZE 256

#define CNVSIZE 64

struct convert
{
    unsigned src;
    unsigned dst;
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
    // context
    struct convert seeds[CNVSIZE];
    struct convert* seed = NULL;
    struct convert* seed_end = NULL;

    // READ SEEDS
    fgets(buffer, BUFSIZE, file);
    cur = buffer;
    // jump
    while (*cur != ' ') cur++;
    // add to array
    seed = seeds;
    while (*cur != '\n') {
        cur++;
        seed->src = scan_unsigned(&cur);
        seed->dst = seed->src;
        seed++;
    }
    seed_end = seed;

    // PROCESS
    while (!feof(file)) {
        // GET ROW
        fgets(buffer, BUFSIZE, file);
        if (!*buffer) break;
        cur = buffer;
        // READING
        if (*cur == '\n');
        // new map case
        if (*cur < '0' || *cur > '9') {
            seed = seeds;
            while (seed != seed_end) {
                seed->src = seed->dst;
                seed++;
            }
            continue;
        }
        // convertion case
        unsigned dst = scan_unsigned(&cur);
        cur++;
        unsigned src = scan_unsigned(&cur);
        int correction = (int)dst - (int)src;
        cur++;
        unsigned length = scan_unsigned(&cur);
        // PROCESSING
        seed = seeds;
        while (seed != seed_end) {
            if (seed->src >= src && seed->src < src+length)
                seed->dst += correction;
            seed++;
        }
        // RESET BUFFER
        *buffer = '\0';
    }

    // FIND MIN LOC
    seed = seeds;
    unsigned min = seed->dst;
    seed++;
    while (seed != seed_end) {
        if (seed->dst < min) min = seed->dst;
        seed++;
    }

    // RESULT
    printf("Min: %u\n", min);

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