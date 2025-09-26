#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NMIN 10
#define NMAX 1000000

void estrazione(int *);
int controlloEstr(int *, int);
void controlloVinc(int *, int *, int *);
void getInt(char *, int *, int, int);
void printAr(int *, int);

int main() {
  /*dichiaro variabili*/
  int vincente[6], N, i, j, giocata[6], freq[4] = {0};
  FILE *fp;
  /*inizializzo seme*/
  srand(time(0));
  /*intro*/
  printf("Questo programa simula il Superenalotto, estraendo e confontando N "
         "sestine con quella vincente. Infine tutte le giocate verrano scritte "
         "sul file ''giocate.dat''\n");
  /*estraggo la sestina vincente*/
  estrazione(vincente);
  printf("Sestina vincente: ");
  printAr(vincente, 6);
  /*acquisico numero di giocate*/
  getInt("Inserire il numero di giocate che vuoi effettuare", &N, NMIN, NMAX);
  /*avvio le giocate e controllo le vittorie*/
  if ((fp = fopen("giocate.dat", "w")) != NULL) {
    for (i = 0; i < N; i++) {
      estrazione(giocata);
      controlloVinc(vincente, giocata, freq);
      fprintf(fp, "Giocata#%d: ", i);
      for (j = 0; j < 6; j++) {
        fprintf(fp, "%d ", giocata[j]);
      }
      fprintf(fp, "\n");
    }
    fclose(fp);
  } else {
    printf("Impossibile aprire il file ''giocate.dat''. Il programma verrà "
           "terminato.");
    exit(EXIT_FAILURE);
  }
  /*stampo le percentuali di vittoria*/
  for (i = 0; i < 4; i++) {
    printf("Perc di %d: %lf\n", i + 3, ((double)(*(freq + i)) * 100) / N);
  }

  exit(0);
} /*fine main*/

void printAr(int *array, int len) {
  int i;
  for (i = 0; i < len; i++) {
    printf("%d\t", *(array + i));
  }
  printf("\n");
} /*fine printar*/

void controlloVinc(int *vincente, int *giocata, int *freq) {
  int i, j, k;
  k = 0;
  for (i = 0; i < 6; i++) {
    for (j = 0; j < 6; j++) {
      if (*(vincente + k) == *(giocata + j))
        k++;
    }
  }
  if (k > 2) {
    printf("Hai vinto con %d numeri uguali. Giocata:\n", k);
    printAr(giocata, 6);
    *(freq + k - 3) += 1;
  }
} /*fine controllo*/

void getInt(char *mess, int *input, int min, int max) {
  do {
    printf("%s tra %d e %d: ", mess, min, max);
    scanf("%d", input);
  } while (*input < min || *input > max);
} /*fine get*/

void estrazione(int *array) {
  int i;
  for (i = 0; i < 6; i++) {
    do {
      *(array + i) = (rand() % 90) + 1;
    } while (controlloEstr(array, i));
  }
} /*fine estrazione*/

int controlloEstr(int *array, int ind) {
  int i;
  for (i = 0; i < ind; i++) {
    if (*(array + i) == *(array + ind))
      return 1;
  }
  return 0;
} /*fine controllo*/
