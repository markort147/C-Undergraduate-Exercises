#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define EPSILON 1e-6

double f(double);
double compute(double, double);
void getInputs(double*, double*);

int main()
{
  printf("Compute the roots of the function f(x)=x^3-2x+4 in the specified range [a,b].\n");

  double a, b;
  getInputs(&a, &b);

  if (f(a) * f(b) > 0)
  {
    printf("There are no solutions within [%lf, %lf]\n", a, b);
    return 0;
  }

  double result = compute(a, b);
  printf("Result: %lf\n", result);

  return 0;
}

void getInputs(double* a, double* b)
{
  do
  {
    printf("Enter lower bound a: ");
    scanf("%lf", a);
    printf("Enter upper bound b: ");
    scanf("%lf", b);
  } while (*b <= *a);
}

double compute(double a, double b)
{
  double result;
  while (fabs(a - b) > EPSILON)
  {
    result = (a + b) / 2;
    double product = f(a) * f(result);

    if (product == 0)
      break;

    if (product > 0)
      a = result;
    else
      b = result;
  }
  return result;
}

double f(double x)
{
  return x * x * x - 2 * x + 4;
}