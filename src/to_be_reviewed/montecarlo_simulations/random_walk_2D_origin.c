#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define T_MAX 10000
#define N_TRA 1000
#define N_SIM 100
#define ORIGIN 0

typedef struct coordinates {
  int x, y;
} coord;

int main() {
  /*seed*/
  unsigned int seed;
  FILE *devrand = fopen("/dev/urandom", "r");
  fread(&seed, sizeof(unsigned int), 1, devrand);
  srand48(seed);
  fclose(devrand);
  /*variables statement*/
  int i, j, k;
  double r, countZero[N_SIM] = {0.}, average, averageSq, fraction, unc;
  coord pos;
  /*randwalk*/
  for (k = 0; k < N_SIM; k++) {
    for (i = 0; i < N_TRA; i++) {
      pos.x = pos.y = ORIGIN;
      for (j = 1; j < T_MAX + 1; j++) {
        if ((r = drand48()) < 0.25)
          pos.x++;
        else if (r < 0.5)
          pos.x--;
        else if (r < 0.75)
          pos.y++;
        else
          pos.y--;
        /*il viandante si ferma se ritorna nell'origine*/
        if (pos.x == ORIGIN && pos.y == ORIGIN) {
          j = T_MAX;
          countZero[k]++;
        }
      }
    }
  }
  average = averageSq = 0.;
  for (i = 0; i < N_SIM; i++) {
    countZero[i] /= (double)N_TRA;
    average += countZero[i];
    averageSq += countZero[i] * countZero[i];
  }
  fraction = average / (double)N_SIM;
  unc = sqrt(((double)N_SIM / (N_SIM - 1)) * (averageSq / (double)N_SIM) -
             fraction * fraction);
  printf("Probabilità di ritorno alle origini su %d passi: %lf +- %lf\n", T_MAX,
         fraction, unc);
  return 0;
}
