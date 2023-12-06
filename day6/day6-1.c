#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

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
    char buffer_time[BUFSIZE];
    char buffer_dist[BUFSIZE];
    char* time_cur = buffer_time;
    char* dist_cur = buffer_dist;
    unsigned long prod = 1;
    // context
    struct race_data races[ARRSIZE];
    struct race_data* race = NULL;
    struct race_data* race_end;

    // READING
    fgets(buffer_time, BUFSIZE, file);
    fgets(buffer_dist, BUFSIZE, file);
    // jump
    while (*time_cur != ' ') time_cur++;
    while (*dist_cur != ' ') dist_cur++;
    // read loop
    race = races;
    while (*time_cur != '\n') {
        // jump
        while (*time_cur == ' ') time_cur++;
        while (*dist_cur == ' ') dist_cur++;
        race->time = scan_unsigned(&time_cur);
        race->dist = scan_unsigned(&dist_cur);
        race++;
    }
    race_end = race;

    // PROCESS
    race = races;
    while (race != race_end) {
        unsigned delta = race->time*race->time - 4*race->dist;
        double sqrt_delta = sqrt(delta);
        double x1 = (-race->time - sqrt_delta) / (-2);
        double x2 = (-race->time + sqrt_delta) / (-2);
        int min = ceil(x1);
        int max = ceil(x2);
        min = (min < 0) ? 0 : min;
        max = (max < 0) ? 0 : max;
    }

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