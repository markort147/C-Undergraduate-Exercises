/*le info da scrivere su istogramma sono inviate su standart output*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define P 0.5
#define TMAX 10000
#define N_TRA 10000
#define T_STEP 2
#define IN_POS 0
#define PRINT_STEP 100
#define LEN 14

int main()
{
  /*len check*/
  if (TMAX > pow(T_STEP, TMAX))
  {
    fprintf(stderr, "ERROR: lunghezza degli array averPos e averSqPos insufficiente\n");
    exit(EXIT_FAILURE);
  }

  /*seed*/
  unsigned int seed;
  FILE *devrand = fopen("/dev/urandom", "r");
  fread(&seed, sizeof(unsigned int), 1, devrand);
  srand48(seed);
  fclose(devrand);

  /*variables statement*/
  int pos, i, j, k, h;
  double averPos[LEN] = {0.}, averSqPos[LEN] = {0.};
  FILE *ftraj, *faver;

  /*randwalk*/
  ftraj = fopen("rw1D_traj.dat", "w");
  faver = fopen("rw1D_aver.dat", "w");
  for (i = 0; i < N_TRA; i++)
  {
    k = T_STEP;
    h = 0;
    pos = IN_POS;
    for (j = 0; j < TMAX; j++)
    {
      if (drand48() < P)
        pos++;
      else
        pos--;
      if ((i % PRINT_STEP) == 0)
      {
        fprintf(ftraj, "%d\t %d\n", j, pos);
      }
      if (j == k)
      {
        averPos[h] += pos;
        averSqPos[h] += pos * pos;
        k *= T_STEP;
        h++;
      }
    }
    fprintf(ftraj, "\n");
  }
  fclose(ftraj);

  /*make average*/
  for (i = 0; i < LEN; i++)
  {
    averPos[i] /= (double)N_TRA;
    averSqPos[i] /= (double)N_TRA;
  }

  /*print average on file*/
  fprintf(faver, "#Medie di x e di x^2 su %d traiettorie \n#\t t\t <x>\t\t <x^2>\n", N_TRA);
  for (i = 1; i < LEN; i++)
  {
    fprintf(faver, "\t %d\t %lf\t %lf\n", (int)pow(T_STEP, i), averPos[i - 1], averSqPos[i - 1]);
  }
  fclose(faver);

  return 0;
}
