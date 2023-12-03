#include <stdio.h>
#include <stdbool.h>

#define BUFSIZE 256

unsigned str_to_integer(const char *str);
const char* str_begin(const char* haystack, const char* needle);

int main(int argc, char* argv[])
{
    //no file exception
    if (argc < 2) {
        printf("[NO FILE GIVEN !]\n");
        return 0;
    }

    // init
    FILE* file = fopen(argv[1], "r");
    char buffer[BUFSIZE];
    char* cur = NULL;
    unsigned sum = 0;

    // process
    while (!feof(file)) {
        fgets(buffer, BUFSIZE, file);
        unsigned red = 0;
        unsigned green = 0;
        unsigned blue = 0;
        unsigned min = 0;
        // stop if end of file
        if (!*buffer) break;
        // get game ID
        cur = buffer + 5;
        unsigned id = str_to_integer(cur);
        // row loop
        while (*cur) {
            // jump
            while (*cur != ' ') cur++;
            cur++;
            // get nbr of cubes
            unsigned nbr = str_to_integer(cur);
            // jump
            while (*cur != ' ') cur++;
            cur++;
            // get disponibility by cube color
            if (*cur == 'r' && nbr > red) red = nbr;
            else if (*cur == 'g' && nbr > green) green = nbr;
            else if (*cur == 'b' && nbr > blue) blue = nbr;
            // jump
            while (*cur && *cur != ',' && *cur != ';') cur++;
        }
        // add square of game min
        min = red * green * blue;
        sum += min;
        // reset buffer
        *buffer = '\0';
    }
    printf("Sum : %u\n", sum);

    // return
    return 0;
}

unsigned str_to_integer(const char *str)
{
    int nbr = 0;
    if (!str) return nbr;
    while (*str >= '0' && *str <= '9') {
        nbr *= 10;
        nbr += *str - '0';
        str++;
    }
    return nbr;
}

const char* str_begin(const char* haystack, const char* needle)
{
    size_t index = 0;
    while (needle[index] && *(haystack + index) == needle[index]) {
        index++;
        if (!*(haystack + index) && needle[index]) return NULL;
    }
    if (!needle[index]) return haystack;
    return NULL;
}