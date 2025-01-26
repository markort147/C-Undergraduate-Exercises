#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define EPSILON 1.e-6

int main()
{
  printf("Compute the root of cos(x) in (0, PI) within an error delta of %.10lf.\n", EPSILON);

  double a, b = M_PI, c;
  double delta = fabs(a - b);
  int i = 1;
  while (delta > EPSILON)
  {
    double p;
    c = 0.5 * (a + b);
    p = cos(a) * cos(c);
    if (p > 0.)
      a = c;
    else if (p < 0.)
      b = c;
    else
      a = b = c;
    i++;
    delta = fabs(a - b);
  }

  printf("Iterative result: %f\n", c);
  printf("Expected result: %f\n", 0.5 * M_PI);
  printf("Computed iterations: %d\n", i);
}
