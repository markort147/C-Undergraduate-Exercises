#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printAr(int*);
void randAr(int*);
void remuxAr(int*, int);
void fillTemp(int*, int*);
int check_duplicates(int*, int);

int main() {
    int colonna, array[40], i;
    char enter;
    srand(time(0));
    printf("\x1b[2J\x1b[H");
    printf("\n");
    fflush(stdout);
    printf("Think of a number between 1 and 40, and press enter.");
    scanf("%c", &enter);
    randAr(array);
    for (i = 0; i < 4; i++) {
        printf("\x1b[2J\x1b[H");
        printf("\n");
        fflush(stdout);
        printAr(array);
        do {
            printf("Enter the column that contains your number: ");
            scanf("%d", &colonna);
        } while (colonna < 1 || colonna > 3);
        remuxAr(array, colonna);
    }
    printf("Your number is: %d\n", array[20]);
    return 0;
}

void remuxAr(int* array, int colonna) {
    int temp[40];

    fillTemp(array, temp);

    if (colonna == 1) {
        for (int i = 0; i < 14; i++) {
            array[i + 13] = temp[i * 3];

            if (i < 13) {
                array[i] = temp[i * 3 + 2];
                array[i + 27] = temp[i * 3 + 1];
            }
        }
    } else if (colonna == 2) {
        for (int i = 0; i < 14; i++) {
            array[i] = temp[i * 3];

            if (i < 13) {
                array[i + 14] = temp[i * 3 + 1];
                array[i + 27] = temp[i * 3 + 2];
            }
        }
    } else if (colonna == 3) {
        for (int i = 0; i < 14; i++) {
            array[i] = temp[i * 3];

            if (i < 13) {
                array[i + 14] = temp[i * 3 + 2];
                array[i + 27] = temp[i * 3 + 1];
            }
        }
    }
}

void fillTemp(int* array, int* temp) {
    for (int i = 0; i < 40; i++) {
        temp[i] = array[i];
    }
}

void printAr(int* array) {
    printf("|   1  |   2  |   3  |\n");
    printf("|======|======|======|\n");

    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 3; j++) {
            printf("|%4d  ", array[i * 3 + j]);
        }
        printf("|\n");
    }
    printf("|%4d  |\n", array[39]);
}

void randAr(int* array) {
    int i;
    for (i = 0; i < 40; i++) {
        do {
            array[i] = (rand() % 40) + 1;
        } while (check_duplicates(array, i));
    }
}

int check_duplicates(int* array, int el) {
    for (int i = 0; i < el; i++) {
        if (array[el] == array[i]) return 1;
    }
    return 0;
}
