#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#define R 1
#define MAXN 6000
#define MAXro 0.0625

double getDoublePos(char *, double);
void genCoor(double *, double *, double);

int main()
{
  /*dichiarazione variabili*/
  int i, j, col, stop;
  double N1, N2, ro, x1[MAXN], y1[MAXN], x2, y2;
  FILE *fp;
  /*inzializzo seme*/
  srand(time(0));
  /*intro*/
  printf("Questo programma simula, in maniera semplificata, la collisione tra due fasci di particelle.\nUna singola collisione avviene quando una particella del primo fascio cade in un cerchio incentrato in una particella del secondo fascio, di raggio definito dall'utente\n");
  /*acquisizione N1, N2 e ro*/
  N1 = getDoublePos("Inserire numero di particelle del primo fascio", MAXN);
  N2 = getDoublePos("Inserire numero di particelle del secondo fascio", MAXN);
  ro = getDoublePos("Inserire raggio", MAXro);
  /*genero coordinate e riempio gli array*/
  genCoor(x1, y1, N1);
  /*scrivo x1 e y1 su file*/
  if ((fp = fopen("beam1.dat", "w")) != NULL)
  {
    for (i = 0; i < N1; i++)
    {
      fprintf(fp, "%lf\t%lf\n", *(x1 + i), *(y1 + i));
    }
    fclose(fp);
  }
  else
  {
    printf("Impossibile aprire il file 'beam1.dat'.\nIl programma proseguirà senza aprire il file.\n");
  }
  /*genero particelle del secondo fascio e verifico le collisioni*/
  col = 0;
  for (i = 0; i < N2; i++)
  {
    do
    {
      x2 = R * (double)rand() / RAND_MAX;
      y2 = R * (double)rand() / RAND_MAX;
    } while (x2 * x2 + y2 * y2 - R > 0);
    j = stop = 0;
    do
    {
      if (sqrt(pow(*(x1 + j) + x2, 2) + pow(*(y1 + j) + y2, 2)) <= ro)
      {
        col++;
        stop = 1;
      }
      j++;
    } while (j < N1 && stop == 0);
  }
  /*stampo risultato a schermo*/
  printf("Numero di collisioni: %d\nPercentuale di collisioni rispetto alle particelle del secondo fascio: %lf\n", col, 100 * col / N2);
  /*uscita*/
  exit(0);
} /*fine main*/

void genCoor(double *x, double *y, double N)
{
  int i;
  for (i = 0; i < N; i++)
  {
    do
    {
      *(x + i) = R * (double)rand() / RAND_MAX;
      *(y + i) = R * (double)rand() / RAND_MAX;
    } while (pow(*(x + i), 2) + pow(*(y + i), 2) - R > 0);
  }
} /*fine genCoor*/

double getDoublePos(char *mess, double max)
{
  double out;
  do
  {
    printf("%s, positivo e minore di %lf:", mess, max);
    scanf("%lf", &out);
  } while (out < 0 || out > max);
  return out;
} /*fine getdoublepos*/
