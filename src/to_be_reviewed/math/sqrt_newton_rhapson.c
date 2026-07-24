#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  printf("SQUARE RADIX NEWTON-RHAPSON CALCULATOR\n");

  int repeat = 1;
  while (repeat) {

    double input = 0.;
    while (input <= 0) {
      printf("Enter a positive number: ");
      scanf("%lf", &input);
    }

    int iterations = 0;
    while (iterations <= 0) {
      printf("Iterations: ");
      scanf("%d", &iterations);
    }

    double rad = input; // initial guess
    for (int i = 0; i < iterations; i++) {
      rad = 0.5 * (rad + input / rad);
    }

    printf("Iterative result: %lf\n", rad);
    printf("Expected value: %lf\n", sqrt(input));

    do {
      printf("Like it? 1 to repeat - 0 to close: ");
      scanf("%i", &repeat);
    } while (repeat != 1 && repeat != 0);
  }

  return 0;
}
