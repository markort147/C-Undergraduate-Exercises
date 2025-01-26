#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main()
{
  double sump, suma, sumg, pestot, mpes, mgeo, marit, valore[100], peso[100];
  int i, n;

  printf("Compute arithmetic, weighted and geomtric means of the given numbers.\n");

  do
  {
    printf("How many numbers (up to 100)? ");
    scanf("%d", &n);
  } while (n <= 0 || n > 100);

  sump = suma = pestot = 0;
  sumg = 1;
  for (i = 0; i < n; i++)
  {
    printf("Enter the number %d: ", i + 1);
    scanf("%lf", &valore[i]);
    do
    {
      printf("Enter the weight (positive): ");
      scanf("%lf", &peso[i]);
    } while (peso <= 0);
    sump += valore[i] * peso[i];
    suma += valore[i];
    sumg *= valore[i];
    pestot += peso[i];
  }

  mpes = sump / pestot;
  mgeo = pow(sumg, 1. / n);
  marit = suma / n;
  printf("Weighted: %lf \nGeometric: %lf \nArithmetic: %lf \n", mpes, mgeo, marit);

  return 0;
}
