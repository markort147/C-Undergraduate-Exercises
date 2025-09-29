#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define P 0.5
#define TMAX 1000
#define N_TRA 10000
#define T_STEP 20
#define IN_POS 0

int main() {
  /*seed*/
  unsigned int seed;
  FILE *devrand = fopen("/dev/urandom", "r");
  fread(&seed, sizeof(unsigned int), 1, devrand);
  srand48(seed);
  fclose(devrand);
  /*variables statement*/
  int pos, i, j, histogram, timeHist;
  FILE *ftraj, *fhist;
  /*per istogramma*/
  fprintf(stderr, "0)Istogramma ad un tempo fissato;\n1)no istogramma.\n: ");
  scanf("%d", &histogram);
  if (histogram == 0) {
    fprintf(stderr, "A quale tempo?\n: ");
    scanf("%d", &timeHist);
    fhist = fopen("rw1D_hist.dat", "w");
  }
  /*randwalk*/
  ftraj = fopen("randwalk1D.dat", "w");
  for (i = 0; i < N_TRA; i++) {
    pos = IN_POS;
    for (j = 0; j < TMAX; j++) {
      if (drand48() < P)
        pos++;
      else
        pos--;
      if (j % T_STEP == 0) {
        fprintf(ftraj, "%d\t %d\n", j, pos);
      }
      if (histogram == 0 && j == timeHist) {
        fprintf(fhist, "%d\n", pos);
      }
    }
  }
  fclose(ftraj);
  fclose(fhist);
  /*per le medie da gnuplot lanciare i comandi:
    >p './rw1D_traj.dat' sm un
    >p './rw1D_traj.dat' u 1:($2**2) sm un
    non ho lasciato un riga vuota (\n) tra una traiettoria e l'altra, altrimenti
    il comando "smooth unique" non avrebbe funzionato*/
  return 0;
}
