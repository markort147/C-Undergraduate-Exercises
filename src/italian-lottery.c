#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    uint64_t state;
    uint64_t increment;
} Pcg32;

int extract_and_check(int[6], const unsigned char[91], Pcg32*);
void extract(int[6], Pcg32*);
void scanPosInt(char*, int*);
void printPlayArray(int[6], int);
static uint32_t pcg32_next(Pcg32*);
static void pcg32_seed(Pcg32*, uint64_t, uint64_t);
static uint32_t pcg32_bounded(Pcg32*, uint32_t);

int main() {
    int winning[6], N, play[6];
    long freqs[5] = {0};
    Pcg32 rng;
    pcg32_seed(&rng, (uint64_t)time(NULL), 54u);

    printf("This app simulates the Italian Superenalotto\n");

    // Winning six
    extract(winning, &rng);
    printf("Winning six: ");
    printPlayArray(winning, 6);
    unsigned char is_winning[91] = {0};
    for (int i = 0; i < 6; i++) {
        is_winning[winning[i]] = 1;
    }

    // Simulation
    scanPosInt("Enter number of plays", &N);
    int next_percent = 1;
    long long next_update = N / 100;
    struct timespec start;
    timespec_get(&start, TIME_UTC);
    printf("\033[?25l");  // Hide cursor
    for (int i = 0; i < N; i++) {
        int matches = extract_and_check(play, is_winning, &rng);
        if (matches >= 2) freqs[matches - 2]++;
        if ((long long)(i + 1) >= next_update) {
            printf("\r\033[KChecked: %d%%", next_percent);
            fflush(stdout);
            next_percent++;
            next_update = (long long)next_percent * N / 100;
        }
    }
    printf("\033[?25h\n");  // Show cursor again

    struct timespec end;
    timespec_get(&end, TIME_UTC);
    double elapsed = (double)(end.tv_sec - start.tv_sec) +
                     (double)(end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Elapsed: %.6f s\n", elapsed);

    // Results
    for (int i = 0; i < 5; i++) {
        long double one_in = (long double)N / (long double)freqs[i];
        printf("Winning %ds:\t%ld (%.2Le%%) - 1 every %.0Lf plays\n", i + 2,
               freqs[i], ((long double)freqs[i] * 100.0L) / N, one_in);
    }
    exit(0);
}

void printPlayArray(int array[6], int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("\t%d", array[i]);
    }
    printf("\n");
}

void scanPosInt(char* mess, int* input) {
    do {
        printf("%s: ", mess);
        scanf("%d", input);
    } while (*input < 0);
}

int extract_and_check(int* play, const unsigned char is_winning[91],
                      Pcg32* rng) {
    int matches = 0;

    for (int i = 0; i < 6; i++) {
        int value, duplicate;

        do {
            value = (int)pcg32_bounded(rng, 90) + 1;
            duplicate = 0;

            for (int j = 0; j < i; j++) {
                if (play[j] == value) {
                    duplicate = 1;
                    break;
                }
            }
        } while (duplicate);

        play[i] = value;
        matches += is_winning[value];
    }

    return matches;
}

void extract(int winning[6], Pcg32* rng) {
    for (int i = 0; i < 6; i++) {
        int value, duplicate;

        do {
            value = (int)pcg32_bounded(rng, 90) + 1;
            duplicate = 0;

            for (int j = 0; j < i; j++) {
                if (winning[j] == value) {
                    duplicate = 1;
                    break;
                }
            }
        } while (duplicate);

        winning[i] = value;
    }
}

static uint32_t pcg32_next(Pcg32* rng) {
    uint64_t old_state = rng->state;

    rng->state = old_state * 6364136223846793005ULL + rng->increment;

    uint32_t xorshifted = (uint32_t)(((old_state >> 18u) ^ old_state) >> 27u);

    uint32_t rotation = (uint32_t)(old_state >> 59u);

    return (xorshifted >> rotation) | (xorshifted << ((-rotation) & 31u));
}

static void pcg32_seed(Pcg32* rng, uint64_t seed, uint64_t sequence) {
    rng->state = 0;
    rng->increment = (sequence << 1u) | 1u;

    pcg32_next(rng);
    rng->state += seed;
    pcg32_next(rng);
}

static uint32_t pcg32_bounded(Pcg32* rng, uint32_t bound) {
    uint32_t threshold = -bound % bound;

    for (;;) {
        uint32_t value = pcg32_next(rng);

        if (value >= threshold) {
            return value % bound;
        }
    }
}
