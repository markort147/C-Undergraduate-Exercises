#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printAr(int *);
void randAr(int *);
void remuxAr(int *, int);
void fillTemp(int *, int *);
int ciSonoCopie(int *, int);

int main() {
  int colonna, array[40], i;
  char enter;
  srand(time(0));
  printf("Think of a number between 1 and 40, and press enter.");
  scanf("%c", &enter);
  randAr(array);
  for (i = 0; i < 4; i++) {
    printAr(array);
    do {
      printf("Enter the column that contains your number: ");
      scanf("%d", &colonna);
    } while (colonna < 1 || colonna > 3);
    remuxAr(array, colonna);
  }
  printf("Your number is: %d\n", array[20]);
  return 0;
} /*fine main*/

void remuxAr(int *array, int colonna) {
  int temp[40], i;
  fillTemp(array, temp);
  if (colonna == 2) {
    for (i = 0; i < 14; i++) {
      *(array + i + 27) = *(temp + i * 3); /*inizio*/
      if (i < 13) {
        *(array + i + 14) = *(temp + i * 3 + 1); /*centro*/
        *(array + i) = *(temp + i * 3 + 2);      /*fine*/
      }
    }
  } else if (colonna == 1) {
    for (i = 0; i < 14; i++) {
      *(array + i + 13) = *(temp + i * 3); /*centro*/
      if (i < 13) {
        *(array + i + 27) = *(temp + i * 3 + 1); /*inizio*/
        *(array + i) = *(temp + i * 3 + 2);      /*fine*/
      }
    }
  } else if (colonna == 3) {
    for (i = 0; i < 14; i++) {
      *(array + i) = *(temp + i * 3); /*inizio*/
      if (i < 13) {
        *(array + i + 27) = *(temp + i * 3 + 1); /*fine*/
        *(array + i + 14) = *(temp + i * 3 + 2); /*centro*/
      }
    }
  }
} /*fine remux*/

void fillTemp(int *array, int *temp) {
  int i;
  for (i = 0; i < 40; i++) {
    *(temp + i) = *(array + i);
  }
} /*fine filltemp*/

void printAr(int *array) {
  int i, j;
  for (i = 0; i < 13; i++) {
    for (j = 0; j < 3; j++) {
      printf("%2d\t", *(array + i * 3 + j));
    }
    printf("\n");
  }
  printf("%d\n", *(array + 39));
} /*fine print in colonne*/

void randAr(int *array) {
  int i;
  for (i = 0; i < 40; i++) {
    do {
      *(array + i) = (rand() % 40) + 1;
    } while (ciSonoCopie(array, i) == 1);
  }
} /*fine riempimento casuale array*/

int ciSonoCopie(int *array, int el) {
  int i;
  for (i = 0; i < el; i++) {
    if (array[el] == array[i])
      return 1;
  }
  return 0;
} /*ciSonoCopie*/
