#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#define BUFSIZE 256

#define ROWSIZE 10
#define MAXNUM 2048
#define MAXSYM 2048

struct number
{
    unsigned value;
    ptrdiff_t i, j;
    ptrdiff_t length;
};

struct symbol
{
    ptrdiff_t i, j;
};

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
    struct number numbers[MAXNUM];
    struct number* num = numbers;
    struct number* end_num = NULL;
    struct symbol symbols[MAXSYM];
    struct symbol* sym = symbols;
    struct symbol* end_sym = NULL;

    // READ LOOP
    size_t row = 0;
    while (!feof(file)) {
        // GET ROW
        fgets(buffer, BUFSIZE, file);
        if (!*buffer) break;
        cur = buffer;
        // PROCESS
        while (*cur != '\n') {
            // number case
            if (*cur >= '0' && *cur <= '9') {
                num->i = row;
                num->j = cur - buffer;
                num->value = *cur - '0';
                num->length = 1;
                cur++;
                while (*cur >= '0' && *cur <= '9') {
                    num->value *= 10;
                    num->value += *cur - '0';
                    num->length++;
                    cur++;
                }
                num++;
            }
            // symbol case
            else if (*cur != '.') {
                sym->i = row;
                sym->j = cur - buffer;
                cur++;
                sym++;
            }
            // empty case
            else cur++;
        }
        // RESET BUFFER
        row++;
        *buffer = '\0';
    }
    end_num = num;
    end_sym = sym;

    // PROCESS LOOP
    num = numbers;
    while (num != end_num) {
        bool part = false;
        sym = symbols;
        while (sym != end_sym) {
            ptrdiff_t di = sym->i - num->i;
            ptrdiff_t dj = sym->j - num->j;
            if ((di >= -1 && di <= 1) && (dj >= -1 && dj <= num->length)) {
                part = true;
                break;
            }
            sym++;
        }
        if (part) sum += num->value;
        num++;
    }

    // RESULT
    printf("Sum: %u\n", sum);

    // return
    return 0;
}