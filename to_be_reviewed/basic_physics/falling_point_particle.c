#include <stdio.h>
#include <stdlib.h>
#define G 9.81

void getDouble(char *, double *);

int main() {
  double mass, v0, h0, kinetic, potential, v, h, t;
  int option;
  getDouble("Enter the mass: ", &mass);
  getDouble("Enter the initial speed: ", &v0);
  getDouble("Enter the initial height: ", &h0);
  do {
    getDouble("Enter the time: ", &t);

    v = v0 - G * t;
    h = h0 - v0 * t - G * t * t / 2;
    kinetic = mass * v * v / 2;
    potential = mass * h * G;

    if (h > 0)
      printf("Kinetic energy: %lf\nPotential energy: %lf\n", kinetic,
             potential);
    else
      printf("The particle reached the ground!\n");

    do {
      printf("1 for changing time, 0 for exit: ");
      scanf("%i", &option);
    } while (option != 0 && option != 1);
  } while (option);
}

void getDouble(char *mess, double *in) {
  printf("%s", mess);
  scanf("%lf", in);
}
