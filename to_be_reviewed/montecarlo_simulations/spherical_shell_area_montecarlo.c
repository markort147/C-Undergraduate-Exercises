#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ATOM 10000
#define RMAX 1

#ifndef M_PI
#define M_PI 3.1415
#endif

double getDouble(char *, int, int);
void genCoor(double pos[ATOM][3]);
void fillPrintDist(double pos[ATOM][3], double *);

int main() {
  /*dichiarazione variabili*/
  double pos[ATOM][3], R1, R2, distance[ATOM], Vsfera, VR1, VR2;
  int min, max, i, j;
  /*inizializzazione seme*/
  srand(time(0));
  /*intro*/
  printf("Data una sfera di raggio R=%d e %d atomi di un gas perfetto, questo "
         "programma calcola quale frazione di atomi si trova nella corona "
         "sferica inclusa tra due sfere di raggio R1 e R2, tali che R1<R2<R.\n",
         RMAX, ATOM);
  /*acquisisco raggi*/
  do {
    R1 = getDouble("Inserire R1", 0, RMAX);
    R2 = getDouble("Inserire R2", 0, RMAX);
    if (R1 > R2)
      printf("Errore: R1 deve essere minore di R2.\n");
  } while (R1 > R2);
  /*genero coordinate*/
  genCoor(pos);
  /*riempio distance e scrivo su file*/
  fillPrintDist(pos, distance);
  /*cerco min e max*/
  min = max = 0;
  for (i = 0; i < ATOM; i++) {
    if (*(distance + min) > *(distance + i))
      min = i;
    if (*(distance + max) < *(distance + i))
      max = i;
  }
  printf("Atomo a minima distanza: x=%lf y=%lf z=%lf.\n", *(*(pos + min)),
         *(*(pos + min) + 1), *(*(pos + min) + 2));
  printf("Atomo a massima distanza: x=%lf y=%lf z=%lf.\n", *(*(pos + max)),
         *(*(pos + max) + 1), *(*(pos + max) + 2));
  /*calcolo frazione*/
  j = 0;
  for (i = 0; i < ATOM; i++) {
    if (*(distance + i) < R2 && *(distance + i) > R1)
      j++;
  }
  Vsfera = 4. / 3 * RMAX * RMAX * RMAX * M_PI;
  VR1 = 4. / 3 * R1 * R1 * R1 * M_PI;
  VR2 = 4. / 3 * R2 * R2 * R2 * M_PI;
  printf("Frazione di atomi compresi nella corona sferica: %d/%d.\nArea della "
         "corona (stimata attraverso la frazione): %lf.\nArea della corona di "
         "preciso: %lf.\n",
         j, ATOM, ((double)j / ATOM) * Vsfera, VR2 - VR1);

  exit(0);
} /*fine main*/

void fillPrintDist(double pos[ATOM][3], double *distance) {
  FILE *fp;
  int i, j;
  double d;
  if ((fp = fopen("distanze.dat", "w")) == NULL) {
    printf("Impossibile aprire il file ''distanze.dat''. Il programma verrà "
           "terminato.\n");
    exit(EXIT_FAILURE);
  }
  for (i = 0; i < ATOM; i++) {
    d = 0;
    for (j = 0; j < 3; j++) {
      d += pow(*(*(pos + i) + j), 2);
    }
    *(distance + i) = sqrt(d);
    fprintf(fp, "ATOMO#%d: %lf\n", i, *(distance + i));
  }
} /*fine fillPrintDist*/

double getDouble(char *mess, int min, int max) {
  double out;
  do {
    printf("%s tra %d e %d: ", mess, min, max);
    scanf("%lf", &out);
  } while (out < min || out > max);
  return out;
} /*fine getdouble*/

void genCoor(double pos[ATOM][3]) {
  int i, j;
  double d;
  for (i = 0; i < ATOM; i++) {
    do {
      d = 0;
      for (j = 0; j < 3; j++) {
        *(*(pos + i) + j) = (double)rand() / RAND_MAX;
        d += pow(*(*(pos + i) + j), 2);
      }
    } while (sqrt(d) > RMAX);
  }
} /*fine gencoor*/
