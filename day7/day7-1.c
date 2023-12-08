#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define BUFSIZE 256

#define ARRSIZE 1024
#define HNDSIZE 5

enum HandType {HIGH, PAIR, PAIR2, THREE, FULL, FOUR, FIVE};

struct hand
{
    int* card;
    int* card_ordered;
    enum HandType type;
    unsigned bid;
};

typedef int (*comp_func_t)(const struct hand* h1, const struct hand* h2);

unsigned scan_unsigned(char** cur);

void bubble_sort(int* data, size_t size);

void hand_init(struct hand* self, int* cards, unsigned bid);
void hand_destroy(struct hand* self);
int get_strength(const char ch);
bool hand_as_five(const struct hand* self);
bool hand_as_four(const struct hand* self);
bool hand_as_full(const struct hand* self);
bool hand_as_three(const struct hand* self);
bool hand_as_pair2(const struct hand* self);
bool hand_as_pair(const struct hand* self);
int hand_comp(const struct hand* h1, const struct hand* h2);

// quick sort
ptrdiff_t hand_partition(struct hand* self, ptrdiff_t i, ptrdiff_t j, comp_func_t func);
void hand_quick_step(struct hand* self, ptrdiff_t i, ptrdiff_t j, comp_func_t func);
void hand_sort(struct hand* self, size_t size, comp_func_t func);

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
    unsigned long sum = 0;
    // context
    struct hand hands[ARRSIZE];
    struct hand* h = hands;
    struct hand* h_end = NULL;

    // PROCESS
    while (!feof(file)) {
        // GET ROW
        fgets(buffer, BUFSIZE, file);
        if (!*buffer) break;
        cur = buffer;
        // READING
        int cards[HNDSIZE];
        for (size_t i=0 ; i < HNDSIZE ; i++) {
            cards[i] = get_strength(*cur);
            cur++;
        }
        cur++;
        unsigned bid = scan_unsigned(&cur);
        hand_init(h, cards, bid);
        h++;
        // RESET BUFFER
        *buffer = '\0';
    }
    h_end = h;

    // SORT ARRAY
    hand_sort(hands, h_end - hands, hand_comp);

    // PROCESS
    h = hands;
    size_t rank = 1;
    while (h != h_end) {
        sum += h->bid * rank;
        rank++;
        h++;
    }

    // RESULT
    printf("Sum: %lu\n", sum);

    // FREE ALLOCATED MEMORY
    h = h_end;
    while (h != h_end) {
        hand_destroy(h);
        h++;
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

void bubble_sort(int* data, size_t size)
{
    bool change;
    int buffer;
    do {
        change = false;
        for (size_t i=1 ; i < size ; i++) {
            if (data[i-1] > data[i]) {
                buffer = data[i-1];
                data[i-1] = data[i];
                data[i] = buffer;
                change = true;
            }
        }
    } while (change);
}

void hand_init(struct hand* self, int* cards, unsigned bid)
{
    self->card = malloc(HNDSIZE * sizeof(int));
    self->card_ordered = malloc(HNDSIZE * sizeof(int));
    memcpy(self->card, cards, HNDSIZE * sizeof(int));
    bubble_sort(cards, HNDSIZE);
    memcpy(self->card_ordered, cards, HNDSIZE * sizeof(int));
    // find hand type
    if (hand_as_five(self)) self->type = FIVE;
    else if (hand_as_four(self)) self->type = FOUR;
    else if (hand_as_full(self)) self->type = FULL;
    else if (hand_as_three(self)) self->type = THREE;
    else if (hand_as_pair2(self)) self->type = PAIR2;
    else if (hand_as_pair(self)) self->type = PAIR;
    else self->type = HIGH;
    // set bid
    self->bid = bid;
}

void hand_destroy(struct hand* self)
{
    free(self->card); self->card = NULL;
    free(self->card_ordered); self->card_ordered = NULL;
}

int get_strength(const char ch)
{
    switch (ch) {
        case 'A': return 12;
        case 'K': return 11;
        case 'Q': return 10;
        case 'J': return 9;
        case 'T': return 8;
        default: return ch - '2';
    }
}

bool hand_as_five(const struct hand* self)
{
    bool res = true;
    int val = *self->card_ordered;
    for (size_t i=1 ; i < HNDSIZE ; i++) {
        res &= (self->card_ordered[i] == val);
    }
    return res;
}

bool hand_as_four(const struct hand* self)
{
    unsigned count = 1  ;
    size_t i = 1;
    while (i < HNDSIZE) {
        count = (self->card_ordered[i-1] == self->card_ordered[i]) ? count+1 : 1;
        if (count == 4) return true;
        i++;
    }
    return false;
}

bool hand_as_full(const struct hand* self)
{
    int v1 = *self->card_ordered;
    int v2 = self->card_ordered[HNDSIZE-1];
    for (size_t i = 1 ; i < HNDSIZE-1 ; i++) {
        if (self->card_ordered[i] != v1 && self->card_ordered[i] != v2) return false;
    }
    return true;
}

bool hand_as_three(const struct hand* self)
{
    unsigned count = 1;
    size_t i = 1;
    while (i < HNDSIZE) {
        count = (self->card_ordered[i-1] == self->card_ordered[i]) ? count+1 : 1;
        if (count == 3) return true;
        i++;
    }
    return false;
}

bool hand_as_pair2(const struct hand* self)
{
    bool first_pair = false;
    for (size_t i=1 ; i < HNDSIZE ; i++) {
        if (self->card_ordered[i-1] == self->card_ordered[i]) {
            if (first_pair) return true;
            first_pair = true;
        }
    }
    return false;
}

bool hand_as_pair(const struct hand* self)
{
    for (size_t i=1 ; i < HNDSIZE ; i++) {
        if (self->card_ordered[i-1] == self->card_ordered[i])
            return true;
    }
    return false;
}

int hand_comp(const struct hand* h1, const struct hand* h2)
{
    int diff = h1->type - h2->type;
    if (diff) return diff;
    // same type case
    for (size_t i=0 ; i < HNDSIZE ; i++) {
        diff = h1->card[i] - h2->card[i];
        if (diff) return diff;
    }
    return 0;
}

ptrdiff_t hand_partition(struct hand* hands, ptrdiff_t i, ptrdiff_t j, comp_func_t func)
{
    struct hand buffer;
    ptrdiff_t cursor = i;
    ptrdiff_t pivot_index = i;
    while (cursor < j) {
        if (func(hands + cursor, hands + j) < 0) {
            buffer = hands[cursor];
            hands[cursor] = hands[pivot_index];
            hands[pivot_index] = buffer;
            pivot_index++;
        }
        cursor++;
    }
    buffer = hands[pivot_index];
    hands[pivot_index] = hands[j];
    hands[j] = buffer;
    return pivot_index;
}

void hand_quick_step(struct hand* hands, ptrdiff_t i, ptrdiff_t j, comp_func_t func)
{
    if (i >= j) return;
    ptrdiff_t pivot = hand_partition(hands, i, j, func);
    hand_quick_step(hands, i, pivot-1, func);
    hand_quick_step(hands, pivot+1, j, func);
}

void hand_sort(struct hand* hands, size_t size, comp_func_t func)
{
    hand_quick_step(hands, 0, size-1, func);
}