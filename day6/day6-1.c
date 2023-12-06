#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#define BUFSIZE 64

#define ARRSIZE 4

struct race_data
{
    unsigned time, dist;
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
    char* time_cur = NULL;
    char* dist_cur = NULL;
    unsigned long prod = 1;
    // context
    struct race_data races[ARRSIZE];
    struct race_data* race = NULL;
    struct race_data* race_end;

    // READING
    

    // RESULT
    printf("Prod: %u\n", prod);

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