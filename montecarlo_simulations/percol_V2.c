#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define EMPTY -1
#define MIN_DENS 0.01
#define MAX_DENS 0.99
#define DENS_STEP 0.1
#define N_CONFIG 10

/*variabili globali*/
int *label, *parent, *size;
int L, sumSqSize, percolatingCluster, numSites, percolVerified;
float averSize;
struct bucket
{
  int *data;
  int numData;
} myBucket;

/*funzioni*/
void resetConfig();
void randInit();
void activeSite();
void allocArrays();
void checkPercol();
void fixParent(int, int);
int clusterOf(int);

int main(int argc, char *argv[])
{
  int i, j, k, percSize, oldSites;
  float dens;
  L = atoi(argv[1]);
  randInit();
  allocArrays();
  printf("#algoritmo di percolazione\n#L\t dens\t per\t normSizePerc\t averSize\n");
  for (i = 0; i < N_CONFIG; i++)
  {
    resetConfig();
    percSize = oldSites = j = 0;
    dens = MIN_DENS;
    while (dens <= MAX_DENS)
    {
      numSites = (int)(dens * L * L);
      for (k = oldSites; k < numSites; k++)
        activeSite();
      checkPercol();
      if (percolVerified)
        percSize = size[percolatingCluster];
      if (numSites - percSize)
        averSize = (double)(sumSqSize - percSize * percSize) / (numSites - percSize);
      else
        averSize = 0;
      printf("%d\t %.2f\t %d\t %f\t %f\n", L, dens, percolVerified, percSize / (float)(L * L), averSize);
      dens = MIN_DENS + DENS_STEP * (++j);
      oldSites = numSites;
    }
  }
  return 0;
}

void checkPercol()
{
  int checkingSite, i, checkingCluster;
  for (i = 0; i < L; i++)
  {
    if ((checkingSite = label[i % L + (L - 1) * L]) != EMPTY)
    {
      if ((checkingCluster = clusterOf(checkingSite)) < L)
      {
        percolVerified = 1;
        percolatingCluster = checkingCluster;
        i = L;
      }
    }
  }
}

void activeSite()
{
  int newSite, up, right, left, down, index;
  /*estraggo elemento casuale dalla bucket*/
  newSite = myBucket.data[(index = rand() % myBucket.numData)];
  myBucket.data[index] = myBucket.data[--myBucket.numData];
  /*attivo il vertice nel reticolo*/
  label[newSite] = newSite;
  /*il sito attivato è inizialmente padre-figlio di se stesso*/
  size[newSite]++;
  sumSqSize++;
  /*vicino di sopra*/
  if ((int)(newSite / L) != L - 1)
  {
    if ((up = label[newSite + L]) != EMPTY)
    {
      fixParent(newSite, clusterOf(up));
    }
  }
  /*vicino di sotto*/
  if ((int)(newSite / L))
  {
    if ((down = label[newSite - L]) != EMPTY)
    {
      fixParent(clusterOf(newSite), clusterOf(down));
    }
  }
  /*vicino di destra*/
  if ((right = label[(newSite + 1) % L + (int)(newSite / L) * L]) != EMPTY)
  {
    fixParent(clusterOf(newSite), clusterOf(right));
  }
  /*vicino di sinistra*/
  if ((left = label[(newSite - 1 + L) % L + (int)(newSite / L) * L]) != EMPTY)
  {
    fixParent(clusterOf(newSite), clusterOf(left));
  }
}

int clusterOf(int site)
{
  while (parent[site] != site)
    site = parent[site];
  return site;
}

void fixParent(int clus1, int clus2)
{
  if (clus1 < clus2)
  {
    parent[clus2] = clus1;
    sumSqSize += 2 * size[clus1] * size[clus2];
    size[clus1] += size[clus2];
  }
  else if (clus1 > clus2)
  {
    parent[clus1] = clus2;
    sumSqSize += 2 * size[clus1] * size[clus2];
    size[clus2] += size[clus1];
  }
}

void randInit()
{
  /*FILE *pfrand;
  unsigned int seed;
  pfrand = fopen("/dev/urandom", "r");
  fread(&seed, sizeof(unsigned int), 1, pfrand);
  srand48(seed);
  fclose(pfrand);*/

  srand(time(NULL));
}

void resetConfig()
{
  int i;
  myBucket.numData = L * L;
  percolVerified = sumSqSize = 0;
  for (i = 0; i < L * L; i++)
  {
    label[i] = EMPTY;
    myBucket.data[i] = parent[i] = i;
    size[i] = 0;
  }
}

void allocArrays()
{
  if ((myBucket.data = (int *)malloc(L * L * sizeof(double))) == NULL)
  {
    fprintf(stderr, "Errore: non è stato possibile allocare memoria dinamica per la bucket\n");
    exit(EXIT_FAILURE);
  }
  if ((label = (int *)malloc(L * L * sizeof(int))) == NULL)
  {
    fprintf(stderr, "Errore: non è stato possibile assegnare memoria dinamica all'array 'label'\n");
    exit(EXIT_FAILURE);
  }
  if ((parent = (int *)malloc(L * L * sizeof(int))) == NULL)
  {
    fprintf(stderr, "Errore: non è stato possibile assegnare memoria dinamica all'array 'parent'\n");
    exit(EXIT_FAILURE);
  }
  if ((size = (int *)malloc(L * L * sizeof(int))) == NULL)
  {
    fprintf(stderr, "Errore: non è stato possibile assegnare memoria dinamica all'array 'size'\n");
    exit(EXIT_FAILURE);
  }
}
