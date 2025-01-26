#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define IN_POS 0
#define T_MAX 5000
#define TRAJECT 3721
#define AVERAGE_LEN 100
#define AVERAGE_STEP 100
#define TIME_HIST 30
#define BINS 61 /*sqrt of TRAJECT*/

/*functions statement*/
void myInit();
void posUpdate();
void averageUpdate(int);
void makeAverage();
void printAverage();
void makeHist();

/*global variables*/
struct floatCoordinates
{
  float x, y;
} averagePos[AVERAGE_LEN], averageSqPos[AVERAGE_LEN];
struct intCoordinates
{
  int x, y;
} pos, posAtTime[TRAJECT];

int main()
{
  /*variables statement*/
  int i, j, h;
  /*random walk 2D*/
  myInit();
  for (j = 0; j < TRAJECT; j++)
  {
    h = 0;
    pos.x = pos.y = IN_POS;
    for (i = 0; i < T_MAX; i++)
    {
      posUpdate();
      if (i == TIME_HIST)
      {
        (posAtTime + j)->x = pos.x;
        (posAtTime + j)->y = pos.y;
      }
      if ((i % AVERAGE_STEP) == 0)
        averageUpdate(h++);
    }
  }
  makeAverage();
  printAverage();
  makeHist();
  return 0;
}

void makeHist()
{
  int i, j;
  float histX[BINS] = {0}, histY[BINS] = {0};
  FILE *pHistX, *pHistY;
  if ((pHistX = fopen("rw2D_hist_x.dat", "w")) == NULL || (pHistY = fopen("rw2D_hist_y.dat", "w")) == NULL)
  {
    fprintf(stderr, "ERROR: could not open ''rw2D_hist_x.dat'' or ''rw2D_hist_y.dat''\n");
    exit(EXIT_FAILURE);
  }
  for (i = 0; i < TRAJECT; i++)
  {
    for (j = 0; j < BINS; j++)
    {
      if ((posAtTime + i)->x == (j - TIME_HIST))
        histX[j]++;
      if ((posAtTime + i)->y == (j - TIME_HIST))
        histY[j]++;
    }
  }
  fprintf(pHistX, "#bin\t n\n");
  fprintf(pHistY, "#bin\t n\n");
  for (i = 0; i < BINS; i++)
  {
    fprintf(pHistX, "%d\t %f\n", i - TIME_HIST, histX[i] / (float)TRAJECT);
    fprintf(pHistY, "%d\t %f\n", i - TIME_HIST, histY[i] / (float)TRAJECT);
  }
  fclose(pHistX);
  fclose(pHistY);
}

void printAverage()
{
  int i;
  FILE *pAverage;
  if ((pAverage = fopen("rw2D_average.dat", "w")) == NULL)
  {
    fprintf(stderr, "ERRORE: could not open ''rw2D_average.dat''\n");
    exit(EXIT_FAILURE);
  }
  fprintf(pAverage, "#t\t average(X)\t average(Y)\t average(X^2)\t average(Y^2)\n");
  for (i = 0; i < AVERAGE_LEN; i++)
  {
    fprintf(pAverage, "%d\t %f\t %f\t %f\t %f\n", (i + 1) * AVERAGE_STEP, (averagePos + i)->x, (averagePos + i)->y, (averageSqPos + i)->x, (averageSqPos + i)->y);
  }
  fclose(pAverage);
}

void makeAverage()
{
  int i;
  for (i = 0; i < AVERAGE_LEN; i++)
  {
    (averagePos + i)->x /= (float)TRAJECT;
    (averagePos + i)->y /= (float)TRAJECT;
    (averageSqPos + i)->x /= (float)TRAJECT;
    (averageSqPos + i)->y /= (float)TRAJECT;
  }
}

void averageUpdate(int i)
{
  (averagePos + i)->x += pos.x;
  (averagePos + i)->y += pos.y;
  (averageSqPos + i)->x += pos.x * pos.x;
  (averageSqPos + i)->y += pos.y * pos.y;
}

void posUpdate()
{
  double randNum;
  if ((randNum = drand48()) < 0.25)
    pos.x++;
  else if (randNum < 0.5)
    pos.x--;
  else if (randNum < 0.75)
    pos.y++;
  else
    pos.y--;
}

void myInit()
{
  int i;
  unsigned int seed;
  FILE *devurand;
  /*random seed*/
  devurand = fopen("/dev/urandom", "r");
  fread(&seed, sizeof(unsigned int), 1, devurand);
  srand48(seed);
  fclose(devurand);
  /*initial position*/
  for (i = 0; i < AVERAGE_LEN; i++)
  {
    (averagePos + i)->x = 0;
    (averagePos + i)->y = 0;
    (averageSqPos + i)->x = 0;
    (averageSqPos + i)->y = 0;
  }
}
