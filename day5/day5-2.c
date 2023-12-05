#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#define BUFSIZE 256

#define CNVSIZE 32

struct convert
{
    unsigned src;
    unsigned dst;
};

struct plant_range
{
    struct convert* plant;
    unsigned size;
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
    struct plant_range seeds[CNVSIZE];
    struct plant_range* seed = NULL;
    struct plant_range* seed_end = NULL;

    // READ SEEDS
    fgets(buffer, BUFSIZE, file);
    cur = buffer;
    // jump
    while (*cur != ' ') cur++;
    // add to array
    seed = seeds;
    while (*cur != '\n') {
        cur++;
        unsigned start = scan_unsigned(&cur);
        cur++;
        seed->size = scan_unsigned(&cur);
        seed->plant = malloc(seed->size * sizeof(struct convert));
        for (unsigned i=0 ; i < seed->size ; i++) {
            seed->plant[i].src = start + i;
            seed->plant[i].dst = start + i;
        }
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
                for (unsigned i=0 ; i < seed->size ; i++) {
                    seed->plant[i].src = seed->plant[i].dst;
                }
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
            for (unsigned i=0 ; i < seed->size ; i++) {
                if (seed->plant[i].src >= src && seed->plant[i].src < src+length)
                    seed->plant[i].dst += correction;
            }
            seed++;
        }
        // RESET BUFFER
        *buffer = '\0';
    }

    // FIND MIN LOC
    seed = seeds;
    unsigned min = seed->plant[0].dst;
    while (seed != seed_end) {
        for (unsigned i=0 ; i < seed->size ; i++) {
            if (seed->plant[i].dst < min) min = seed->plant[i].dst;
        }
        seed++;
    }

    // RESULT
    printf("Min: %u\n", min);

    // FREE ALLOCATED MEMORY
    seed = seeds;
    while (seed != seed_end) {
        free(seed->plant);
        seed++;
    }

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