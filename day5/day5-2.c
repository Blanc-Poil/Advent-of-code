#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#define BUFSIZE 256

#define SEESIZE 32
#define MAPSIZE 64
#define PIPSIZE 7

struct seed_range
{
    unsigned start;
    unsigned length;
};

struct convert_rule
{
    unsigned start;
    unsigned length;
    int correction;
};

struct convert_map
{
    struct convert_rule* rules;
    struct convert_rule* end;
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
    struct seed_range seeds[SEESIZE];
    struct seed_range* seed = NULL;
    struct seed_range* seed_end = NULL;
    struct convert_map pipeline[PIPSIZE];
    struct convert_map* map = NULL;
    struct convert_map* map_end = NULL;

    // INIT PIPELINE
    for (size_t i=0 ; i < PIPSIZE ; i++) {
        pipeline[i].end = pipeline[i].rules = malloc(MAPSIZE * sizeof(struct convert_rule));
    }

    // READ SEEDS
    // file read
    fgets(buffer, BUFSIZE, file);
    cur = buffer;
    // jump
    while (*cur != ' ') cur++;
    // read seeds
    seed = seeds;
    while (*cur != '\n') {
        cur++;
        seed->start = scan_unsigned(&cur);
        cur++;
        seed->length = scan_unsigned(&cur);
        seed++;
    }
    seed_end = seed;

    // READ CONVERTION MAPS
    map = pipeline - 1;
    while (!feof(file)) {
        // GET ROW
        fgets(buffer, BUFSIZE, file);
        if (!*buffer) break;
        cur = buffer;
        // READING
        if (*cur == '\n') continue;
        // new map case
        if (*cur < '0' || *cur > '9') {
            map++;
            continue;
        }
        // convert rule case
        unsigned dst = scan_unsigned(&cur);
        cur++;
        map->end->start = scan_unsigned(&cur);
        cur++;
        map->end->length = scan_unsigned(&cur);
        map->end->correction = dst - map->end->start;
        map->end++;
        // RESET BUFFER
        *buffer = '\0';
    }
    map_end = map+1;

    // FIND MIN LOC
    unsigned min = ~0;
    seed = seeds;
    while (seed != seed_end) {
        printf("[PASSED]\n");
        for (size_t i=0 ; i < seed->length ; i++) {
            unsigned value = seed->start + i;
            map = pipeline;
            while (map != map_end) {
                struct convert_rule* rule = map->rules;
                while (rule != map->end) {
                    if (value >= rule->start && value < rule->start + rule->length) {
                        value += rule->correction;
                        break;
                    }
                    rule++;
                }
                map++;
            }
            if (value < min) min = value;
        }
        seed++;
    }

    // RESULT
    printf("Min: %u\n", min);

    // FREE ALLOCATED MEMORY
    map = pipeline;
    while (map != map_end) {
        free(map->rules);
        map++;
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