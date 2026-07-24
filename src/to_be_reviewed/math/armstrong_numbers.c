#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define N_MAX 1000

int getInt(char *, int, int);
int checkC(int);
void stampaFile(int *, int);
void minMax(int *, int);

int main() {
  /*dichiarazione variabili*/
  int nmax, i, n, val, numeriC[N_MAX];
  /*intro*/
  printf("Questo programma cerca tutti quei numeri interi naturali minori di "
         "Nmax che risultano essere la somma dei cubi delle cifre decimali che "
         "li compongono, li scrive in un file ''numeriC.dat'' e stampa a "
         "schermo il numero max e il numero min trovati.\n");
  /*acquisico nmax*/
  nmax = getInt("Inserire Nmax", 0, N_MAX);
  /*ciclo per riempire l'array numeriC*/
  n = 0;
  for (i = 0; i < nmax; i++) {
    val = checkC(i);
    if (val) {
      *(numeriC + n) = i;
      n++;
    }
  }
  /*scrivo su file*/
  stampaFile(numeriC, n);
  /*calcolo min e max e stampo a schermo*/
  minMax(numeriC, n);

  exit(0);
} /*fine main*/

void minMax(int *array, int len) {
  int i, min, max;
  min = max = 0;
  for (i = 0; i < len; i++) {
    if (*(array + min) > *(array + i))
      min = i;
    if (*(array + max) < *(array + i))
      max = i;
  }
  printf("Min: %d  Max: %d\n", *(array + min), *(array + max));
} /*fine minmax*/

void stampaFile(int *array, int len) {
  FILE *fp;
  int i, j, k, l;
  if ((fp = fopen("numeriC.dat", "w")) == NULL) {
    printf("Errore: impossibile aprire numeriC.dat\n");
    exit(EXIT_FAILURE);
  }
  for (l = 0; l < len; l++) {
    for (i = 0; i < 9; i++) {
      for (j = 0; j < 9; j++) {
        for (k = 0; k < 9; k++) {
          if (*(array + l) == i + j * 10 + k * 100) {
            fprintf(fp, "%d = %d**3 + %d**3 + %d**3\n", *(array + l), k, j, i);
          }
        }
      }
    }
  }
  fclose(fp);
} /*fine stampafile*/

int checkC(int n) {
  int i, j, k;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      for (k = 0; k < 9; k++) {
        if (n == i + j * 10 + k * 100) {
          if (n == pow(i, 3) + pow(j, 3) + pow(k, 3)) {
            return 1;
          } else {
            return 0;
          }
        }
      }
    }
  }
  return 0;
} /*getint*/

int getInt(char *mess, int min, int max) {
  int out;
  do {
    printf("%s tra %d e %d: ", mess, min, max);
    scanf("%d", &out);
  } while (out > max || out < min);
  return out;
} /*fine getint*/
