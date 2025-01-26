#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define P_MIN 0.1
#define P_MAX 0.9
#define P_STEP 0.01
#define EMPTY -1
#define N_CONFIG 700
/*#define DEBUG*/
/*#define DEBUG_UPDATELABELS*/

/*dichiarazione variabili globali*/
int *label;
int L;

/*dichiarazione struct*/
typedef struct info
{
  int perc, sizeperc;
  double sizemed;
} info;

/*funzioni*/
void clearlabels();
void makelabels(float);
int updatelabels();
info algorithm();

int main(int argc, char *argv[])
{
  /*dichiarazione variabili*/
  int i, operations, j, count;
  float p;
  info output;
  L = atoi(argv[1]);
  if ((label = (int *)malloc(L * L * sizeof(int))) == NULL)
  {
    fprintf(stderr, "Errore: non è stato possibile assegnare memoria dinamica all'array 'label'\n");
    exit(EXIT_FAILURE);
  }
  /*inizializzazione generatore rand*/
  // FILE *prand;
  // unsigned int seed;
  // prand = fopen("/dev/urandom", "r");
  // fread(&seed, sizeof(unsigned int), 1, prand);
  // srand(seed);
  // fclose(prand);
  srand(time(NULL));
  /*inizio programma*/
  printf("#algoritmo di percolazione\n#L\t p\t per\t Sper\t Smed\t operations\n");
  i = 1;
  p = P_MIN;
  while (p <= P_MAX)
  {
    for (j = 0; j < N_CONFIG; j++)
    {
      operations = 0;
      clearlabels();
      makelabels(p);
#ifdef DEBUG
      if (L == 10 && p == (float)0.51 && j == 0)
      {
        for (i = 0; i < L * L; i++)
        {
          fprintf(stderr, "%d\t", label[i]);
          if (i % L == L - 1)
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "\n");
      }
#endif
      while ((count = updatelabels()))
      {
        operations += count;
        count = 0;
      }
#ifdef DEBUG
      if (L == 10 && p == (float)0.51 && j == 0)
      {
        for (i = 0; i < L * L; i++)
        {
          fprintf(stderr, "%d\t", label[i]);
          if (i % L == L - 1)
            fprintf(stderr, "\n");
        }
      }
#endif
      output = algorithm();
      printf("%d\t %.2f\t %d\t %d\t %lf\t %d\n", L, p, output.perc, output.sizeperc, output.sizemed, operations);
    }
    p = P_MIN + P_STEP * (i++);
  }
  return 0;
}

info algorithm()
{
  info output;
  int i, labelperc, temp, sum, sumsq;
  int *count;
  if ((count = (int *)calloc(L * L, sizeof(int))) == NULL)
  {
    fprintf(stderr, "Errore: non è stato possibile allocare memoria dinamica all'array 'count'\n");
    exit(EXIT_FAILURE);
  }
  /*controllo se percola*/
  output.perc = 0;
  for (i = 0; i < L; i++)
  {
    if ((temp = label[i % L + (L - 1) * L]) != EMPTY && temp < L)
    {
      output.perc = 1;
      labelperc = temp;
      i = L;
    }
  }
  /*dimensione del percolante e dimensione caratteristica dei cluster*/
  output.sizeperc = 0;
  for (i = 0; i < L * L; i++)
  {
    if (label[i] != EMPTY && label[i] != labelperc)
      count[label[i]]++;
    if (labelperc == label[i] && output.perc)
      output.sizeperc++;
  }
  sum = sumsq = 0;
  for (i = 0; i < L * L; i++)
  {
    sum += count[i];
    sumsq += count[i] * count[i];
  }
  if (sum)
    output.sizemed = sumsq / (double)sum;
  return output;
}

int updatelabels()
{
  int check = 0, i;
  int right, over, site;
  for (i = 0; i < L * L; i++)
  {
    site = label[i];
    if (site != EMPTY)
    {
      /*controllo il vicino di destra*/
      right = label[(i + 1) % L + (int)(i / L) * L];
      if (right != EMPTY && site != right)
      {
        if (site > right)
          label[i] = label[(i + 1) % L + (int)(i / L) * L];
        else
          label[(i + 1) % L + (int)(i / L) * L] = label[i];
        check++;
      }
      /*controllo il vicino di sopra*/
      if (((int)(i / L)) != L - 1)
      {
        over = label[i + L];
        if (over != EMPTY && site != over)
        {
          if (site > over)
            label[i] = label[i + L];
          else
            label[i + L] = label[i];
          check++;
        }
      }
    }
  }
#ifdef DEBUG_UPDATELABELS
  int j;
  for (i = 0; i < L; i++)
  {

    for (j = 0; j < L; j++)
    {
      printf("%d ", label[i + j * L]);
    }
    printf("\n");
  }
  printf("\n");
  printf("\n");
#endif
  return check;
}

void clearlabels()
{
  int i;
  for (i = 0; i < L * L; i++)
  {
    *(label + i) = EMPTY;
  }
}

void makelabels(float p)
{
  int i;
  for (i = 0; i < L * L; i++)
  {
    if (rand() / (double)RAND_MAX < p)
      *(label + i) = i;
  }
}
