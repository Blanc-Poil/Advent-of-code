#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

#define BUFSIZE 64

struct race_data
{
    unsigned long time, dist;
};

unsigned long scan_unsigned(char** cur);

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
    struct race_data* race = malloc(sizeof(struct race_data));

    // READING
    fgets(buffer_time, BUFSIZE, file);
    fgets(buffer_dist, BUFSIZE, file);
    // jump
    while (*time_cur != ' ') time_cur++;
    while (*time_cur == ' ') time_cur++;
    while (*dist_cur != ' ') dist_cur++;
    while (*dist_cur == ' ') dist_cur++;
    // read
    race->time = scan_unsigned(&time_cur);
    race->dist = scan_unsigned(&dist_cur);

    // PROCESS
    double t = (double)race->time;
    double r = (double)race->dist;
    double delta = t*t - 4*r;
    double sqrt_delta = sqrt(delta);
    double x1 = (-t + sqrt_delta) / (-2);
    double x2 = (-t - sqrt_delta) / (-2);
    int min = floor(x1) + 1;
    int max = ceil(x2) - 1;
    int ways = max - min + 1;
    prod *= ways;

    // FREE ALLOCATED MEMORY
    free(race);

    // RESULT
    printf("Prod: %lu\n", prod);

    // return
    return 0;
}

unsigned long scan_unsigned(char** cur)
{
    unsigned long value = 0;
    while (**cur >= '0' && **cur <= '9') {
        value *= 10;
        value += **cur - '0';
        (*cur)++;
    }
    return value;
}