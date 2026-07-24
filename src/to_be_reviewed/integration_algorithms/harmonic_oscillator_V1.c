#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif


struct param getData(double *, double *);
double accel(double, double, double);
void euler(struct param, double, double);
void eulercrom(struct param, double, double);
void cPoint(struct param, double, double);
void leapFrog(struct param, double, double);
void velVerlet(struct param, double, double);
void rk4(struct param, double, double);

struct param {
  double m, k, x0, v0;
};

int main() {
  int option;
  struct param osc;
  double dt, tmax;
  osc = getData(&dt, &tmax);
  fprintf(stderr, "0 per eulero, 1 per eulero-cromer, 2 per central point, 3 "
                  "per leap frog, 4 per velocity verlet, 5 per RK4: ");
  scanf("%d", &option);
  if (option == 5) {
    rk4(osc, dt, tmax);
  } else if (option == 4) {
    velVerlet(osc, dt, tmax);
  } else if (option == 3) {
    leapFrog(osc, dt, tmax);
  } else if (option == 2) {
    cPoint(osc, dt, tmax);
  } else if (option == 0) {
    euler(osc, dt, tmax);
  } else if (option == 1) {
    eulercrom(osc, dt, tmax);
  }
  return 0;
} /*main*/

struct param getData(double *dt, double *tmax) {
  double p;
  struct param out;
  fprintf(stderr, "Massa: ");
  scanf("%lf", &out.m);
  fprintf(stderr, "k: ");
  scanf("%lf", &out.k);
  p = 2 * M_PI * sqrt(out.m / out.k);
  fprintf(stderr, "x0: ");
  scanf("%lf", &out.x0);
  fprintf(stderr, "v0: ");
  scanf("%lf", &out.v0);
  fprintf(stderr, "Periodo: %lf. Delta t consigliato: tra %lf e %lf.\n", p,
          p / 20, p / 100);
  fprintf(stderr, "dt: ");
  scanf("%lf", dt);
  fprintf(stderr, "tmax: ");
  scanf("%lf", tmax);
  return out;
} /*getData*/

double accel(double k, double x, double m) { return -k * x / m; } /*accel*/

void euler(struct param in, double dt, double tmax) {
  double xtemp, x = in.x0, v = in.v0, dE, E0;
  int i, N;
  printf("#Algoritmo di Eulero \n#K=%lf m=%lf x0=%lf v0=%lf dt=%lf \n#Colonne: "
         "\n#1:t\t2:x\t3:v\t4:(E-E0)/E0 \n",
         in.k, in.m, in.x0, in.v0, dt);
  N = (int)(tmax / dt);
  E0 = v * v * in.m / 2 - in.k * x * x / 2;
  for (i = 0; i < N + 1; i++) {
    xtemp = x;
    x = x + v * dt;
    v = v + accel(xtemp, in.k, in.m) * dt;
    dE = (v * v * in.m / 2 - in.k * x * x / 2 - E0) / E0;
    printf("%lf\t%lf\t%lf\t%lf\n", i * dt, x, v, dE);
  }
  printf("\n");
} /*euler*/

void eulercrom(struct param in, double dt, double tmax) {
  double x = in.x0, v = in.v0, dE, E0;
  int i, N;
  printf("#Algoritmo di Eulero-Cromer \n#K=%lf m=%lf x0=%lf v0=%lf dt=%lf "
         "\n#Colonne: \n#1:t\t2:x\t3:v\t4:(E-E0)/E0 \n",
         in.k, in.m, in.x0, in.v0, dt);
  N = (int)(tmax / dt);
  E0 = v * v * in.m / 2 - in.k * x * x / 2;
  for (i = 0; i < N + 1; i++) {
    v = v + accel(x, in.k, in.m) * dt;
    x = x + v * dt;
    dE = (v * v * in.m / 2 - in.k * x * x / 2 - E0) / E0;
    printf("%lf\t%lf\t%lf\t%lf\n", i * dt, x, v, dE);
  }
  printf("\n");
} /*eulercrom*/

void cPoint(struct param in, double dt, double tmax) {
  double x = in.x0, v1 = in.v0, v2, dE, E0;
  int i, N;
  printf("#Algoritmo Central Point \n#K=%lf m=%lf x0=%lf v0=%lf dt=%lf "
         "\n#Colonne: \n#1:t\t2:x\t3:v\t4:(E-E0)/E0 \n",
         in.k, in.m, in.x0, in.v0, dt);
  N = (int)(tmax / dt);
  E0 = v1 * v1 * in.m / 2 - in.k * x * x / 2;
  for (i = 0; i < N + 1; i++) {
    v2 = v1 + accel(x, in.k, in.m) * dt;
    x = x + (v2 + v1) * dt / 2;
    dE = (v2 * v2 * in.m / 2 - in.k * x * x / 2 - E0) / E0;
    printf("%lf\t%lf\t%lf\t%lf\n", i * dt, x, v2, dE);
    v1 = v2;
  }
  printf("\n");
} /*cpoint*/

void leapFrog(struct param in, double dt, double tmax) {
  double x = in.x0, v1 = in.v0, v2, dE, E0;
  int i, N;
  printf("#Algoritmo Leap Frog \n#K=%lf m=%lf x0=%lf v0=%lf dt=%lf \n#Colonne: "
         "\n#1:t\t2:x\t3:v\t4:(E-E0)/E0 \n",
         in.k, in.m, in.x0, in.v0, dt);
  N = (int)(tmax / dt);
  E0 = v1 * v1 * in.m / 2 - in.k * x * x / 2;
  v1 = in.v0 + accel(x, in.k, in.m) * dt / 2;
  for (i = 0; i < N + 1; i++) {
    v2 = v1 + accel(x, in.k, in.m) * dt;
    x = x + v2 * dt / 2;
    dE = (v2 * v2 * in.m / 2 - in.k * x * x / 2 - E0) / E0;
    printf("%lf\t%lf\t%lf\t%lf\n", i * dt, x, v2, dE);
    v1 = v2;
  }
  printf("\n");
} /*leapfrog*/

void velVerlet(struct param in, double dt, double tmax) {
  double x1 = in.x0, x2, v = in.v0, dE, E0;
  int i, N;
  printf("#Algoritmo Leap Frog \n#K=%lf m=%lf x0=%lf v0=%lf dt=%lf \n#Colonne: "
         "\n#1:t\t2:x\t3:v\t4:(E-E0)/E0 \n",
         in.k, in.m, in.x0, in.v0, dt);
  N = (int)(tmax / dt);
  E0 = v * v * in.m / 2 - in.k * x1 * x1 / 2;
  for (i = 0; i < N + 1; i++) {
    x2 = x1 + v * dt + accel(x1, in.k, in.m) * dt * dt / 2;
    v = v + (accel(x2, in.k, in.m) + accel(x1, in.k, in.m)) * dt / 2;
    dE = (v * v * in.m / 2 - in.k * x2 * x2 / 2 - E0) / E0;
    printf("%lf\t%lf\t%lf\t%lf\n", i * dt, x2, v, dE);
    x1 = x2;
  }
  printf("\n");
} /*verlet*/

void rk4(struct param in, double dt, double tmax) {
  double x1 = in.x0, x2, v = in.v0, dE, E0;
  double k1, k2, k3, k4, l1, l2, l3, l4;
  int i, N;
  printf("#Algoritmo Runge Kutta 4 \n#K=%lf m=%lf x0=%lf v0=%lf dt=%lf "
         "\n#Colonne: \n#1:t\t2:x\t3:v\t4:(E-E0)/E0 \n",
         in.k, in.m, in.x0, in.v0, dt);
  N = (int)(tmax / dt);
  E0 = v * v * in.m / 2 - in.k * x1 * x1 / 2;
  for (i = 0; i < N + 1; i++) {
    k1 = v;
    l1 = accel(x1, in.k, in.m);
    k2 = v + l1 * dt / 2;
    l2 = accel(x1 + k1 * dt / 2, in.k, in.m);
    k3 = v + l2 * dt / 2;
    l3 = accel(x1 + k2 * dt / 2, in.k, in.m);
    k4 = v + l3 * dt;
    l4 = accel(x1 + k3 * dt, in.k, in.m);
    x2 = x1 + (k1 + 2 * (k2 + k3) + k4) * dt / 6;
    v = v + (l1 + 2 * (l2 + l3) + l4) * dt / 6;
    dE = (v * v * in.m / 2 - in.k * x2 * x2 / 2 - E0) / E0;
    printf("%lf\t%lf\t%lf\t%lf\n", i * dt, x2, v, dE);
    x1 = x2;
  }
  printf("\n");
} /*RK4*/
