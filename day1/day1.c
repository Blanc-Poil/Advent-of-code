#include <stdio.h>

#define BUFSIZE 256

const char* str_begin(const char* haystack, const char* needle);
unsigned spelled_digit(const char* str);

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
    unsigned sum = 0;

    // process
    while (!feof(file)) {
        fgets(buffer, BUFSIZE, file);
        char first = '\0';
        char last = '\0';
        char* cursor = buffer;
        while (*cursor != '\n' && *cursor != '\0') {
            if (*cursor >= '0' && *cursor <= '9') {
                if (!first) first = *cursor - '0';
                last = *cursor - '0';
            }
            unsigned spelled = spelled_digit(cursor);
            if (spelled) {
                if (!first) first = spelled;
                last = spelled;
            }
            cursor++;
        }
        if (first) {
            sum += 10*(first) + last;
        }
        *buffer = '\n';
    }
    printf("Sum : %u\n", sum);

    // return
    return 0;
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

unsigned spelled_digit(const char* str)
{
    char one[] = "one";    
    char two[] = "two";
    char three[] = "three";
    char four[] = "four";
    char five[] = "five";
    char six[] = "six";
    char seven[] = "seven";
    char eight[] = "eight";
    char nine[] = "nine";
    if (str_begin(str, one)) return 1;
    if (str_begin(str, two)) return 2;
    if (str_begin(str, three)) return 3;
    if (str_begin(str, four)) return 4;
    if (str_begin(str, five)) return 5;
    if (str_begin(str, six)) return 6;
    if (str_begin(str, seven)) return 7;
    if (str_begin(str, eight)) return 8;
    if (str_begin(str, nine)) return 9;
    return 0;
}