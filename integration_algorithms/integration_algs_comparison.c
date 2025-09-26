#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define PERIODS 4
#define FRACTION 100000

/*struct param definition*/
typedef struct param {
  double k, m, x_0, v_0;
} param;

/*functions statement*/
double Euler(param, double, double);
double VelocityVerlet(param, double, double);
double RungeKutta4(param, double, double);
double acceleration(double, param);

/*main function*/
int main() {
  /*variables statement*/
  int i;
  param inPar;
  double dT, Tmax, ener[3], expectedPeriod;
  printf("#Comparing energy error of Euler, Velocity Verlet and Runge Kutta 4. "
         "\n#\tdT/period\tEuler\tVelVerlet\tRK4\n");
  /*get data*/
  fprintf(stderr, "k: ");
  scanf("%lf", &inPar.k);
  fprintf(stderr, "m: ");
  scanf("%lf", &inPar.m);
  fprintf(stderr, "x_0: ");
  scanf("%lf", &inPar.x_0);
  fprintf(stderr, "v_0: ");
  scanf("%lf", &inPar.v_0);
  expectedPeriod = 2 * M_PI * inPar.m / inPar.k;
  fprintf(stderr, "Expected period: %lfs\n", expectedPeriod);
  Tmax = PERIODS * expectedPeriod;
  /*run algorithms*/
  i = 1;
  dT = 0;
  while (dT < expectedPeriod) {
    dT = (double)(i * expectedPeriod / FRACTION);
    printf("\t%lf", dT);
    ener[0] = Euler(inPar, dT, Tmax);
    ener[1] = VelocityVerlet(inPar, dT, Tmax);
    ener[2] = RungeKutta4(inPar, dT, Tmax);
    printf("\t%lf\t%lf\t%lf\n", ener[0], ener[1], ener[2]);
    if (i == 1)
      i = 0;
    i += 1000;
  }
  return 0;
}

/*function Euler*/
double Euler(struct param inPar, double dt, double tmax) {
  double xtemp, x = inPar.x_0, v = inPar.v_0, dE, E_0;
  int i, N;
  N = (int)(tmax / dt);
  E_0 = v * v * inPar.m / 2 - inPar.k * x * x / 2;
  for (i = 0; i < N + 1; i++) {
    xtemp = x;
    x = x + v * dt;
    v = v + acceleration(xtemp, inPar) * dt;
  }
  dE = (v * v * inPar.m / 2 - inPar.k * x * x / 2 - E_0) / E_0;
  return dE;
}

/*function Velocity Verlet*/
double VelocityVerlet(param inPar, double dt, double tmax) {
  double x1 = inPar.x_0, x2, v = inPar.v_0, dE, E_0;
  int i, N;
  N = (int)(tmax / dt);
  E_0 = v * v * inPar.m / 2 - inPar.k * x1 * x1 / 2;
  for (i = 0; i < N + 1; i++) {
    x2 = x1 + v * dt + acceleration(x1, inPar) * dt * dt / 2;
    v = v + (acceleration(x2, inPar) + acceleration(x1, inPar)) * dt / 2;
    x1 = x2;
  }
  dE = (v * v * inPar.m / 2 - inPar.k * x2 * x2 / 2 - E_0) / E_0;
  return dE;
}

/*function Runge Kutta 4*/
double RungeKutta4(param inPar, double dt, double tmax) {
  double x1 = inPar.x_0, x2, v = inPar.v_0, dE, E_0;
  double k1, k2, k3, k4, l1, l2, l3, l4;
  int i, N;
  N = (int)(tmax / dt);
  E_0 = v * v * inPar.m / 2 - inPar.k * x1 * x1 / 2;
  for (i = 0; i < N + 1; i++) {
    k1 = v;
    l1 = acceleration(x1, inPar);
    k2 = v + l1 * dt / 2;
    l2 = acceleration(x1 + k1 * dt / 2, inPar);
    k3 = v + l2 * dt / 2;
    l3 = acceleration(x1 + k2 * dt / 2, inPar);
    k4 = v + l3 * dt;
    l4 = acceleration(x1 + k3 * dt, inPar);
    x2 = x1 + (k1 + 2 * (k2 + k3) + k4) * dt / 6;
    v = v + (l1 + 2 * (l2 + l3) + l4) * dt / 6;
    x1 = x2;
  }
  dE = (v * v * inPar.m / 2 - inPar.k * x2 * x2 / 2 - E_0) / E_0;
  return dE;
}

/*function acceleration*/
double acceleration(double x, param inPar) {
  return -inPar.k * x / inPar.m;
} /*accel*/
