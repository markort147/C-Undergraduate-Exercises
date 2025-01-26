#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define EULER_CROM 0
#define RUNGE_KUTTA_4 4
#define VEL_VERLET 3
#define CENTRAL_POINT 1
#define TEST 5

typedef struct param
{
  double m, k, x0, v0;
} param;
typedef struct posTime
{
  double x, t;
} posTime;

param getData(double *, double *, int *);
double accel(double, param);
void eulercrom(param, double, double, posTime *);
void centralPoint(param, double, double, posTime *);
void velVerlet(param, double, double, posTime *);
void rungekutta4(param, double, double, posTime *);
void choiceAlg(param, double, double, posTime *, int);
void period(posTime *, int);

int main()
{
  param initParam;
  double dt, tmax;
  posTime *XandT;
  int numPassi, test;
  initParam = getData(&dt, &tmax, &test);
  numPassi = (int)(tmax / dt) + 1;
  if ((XandT = (posTime *)calloc(numPassi, sizeof(posTime))) == NULL)
  {
    fprintf(stderr, "ERRORE: non e' stato possibile allocare memoria, calcolo del periodo non effettuabile.\n");
  }
  choiceAlg(initParam, dt, tmax, XandT, test);
  period(XandT, numPassi);
  return 0;
} /*main*/

void period(posTime *XandT, int N)
{
  int i, j = 0, jump;
  double T_0, T_1, T, t0, t1, x0, x1;
  fprintf(stderr, "Periodi:\n");
  for (i = 0; i < N - 1; i++)
  {
    x0 = (*(XandT + i)).x;
    x1 = (*(XandT + i + 1)).x;
    if (x1 * x0 <= 0 && jump)
    {
      t0 = (*(XandT + i)).t;
      t1 = (*(XandT + i)).t;
      T_1 = t0 - (t1 - t0) * x0 / (x1 - x0);
      if (j)
      {
        T = 2 * (T_1 - T_0);
        fprintf(stderr, "%lf\n", T);
      }
      T_0 = T_1;
      j = 1;
      if (x1 * x0 == 0)
      {
        jump = 0;
      }
    }
    else if (jump == 0)
    {
      jump = 1;
    }
  }
  fprintf(stderr, "\n");
} /*period*/

void choiceAlg(param initParam, double dt, double tmax, posTime *XandT, int test)
{
  unsigned int option;
  fprintf(stderr, "0 eulero-cromer, 1 per central point, 3 per velocity verlet, 4 per RK4: ");
  scanf("%d", &option);
  if (option == RUNGE_KUTTA_4)
  {
    rungekutta4(initParam, dt, tmax, XandT);
  }
  else if (option == VEL_VERLET)
  {
    velVerlet(initParam, dt, tmax, XandT);
  }
  else if (option == CENTRAL_POINT)
  {
    centralPoint(initParam, dt, tmax, XandT);
  }
  else if (option == EULER_CROM)
  {
    eulercrom(initParam, dt, tmax, XandT);
  }
  else
  {
    fprintf(stderr, "ERRORE: il numero inserito non corrisponde ad alcun algoritmo\n");
    exit(EXIT_FAILURE);
  }
}

param getData(double *dt, double *tmax, int *test)
{
  double p;
  param out;
  fprintf(stderr, "Massa: ");
  scanf("%lf", &out.m);
  fprintf(stderr, "k: ");
  scanf("%lf", &out.k);
  fprintf(stderr, "x0: ");
  scanf("%lf", &out.x0);
  fprintf(stderr, "v0: ");
  scanf("%lf", &out.v0);
  fprintf(stderr, "0 se vuoi effettuare test dell'errore, 1 se vuoi procedere normalmente: ");
  scanf("%d", test);
  if (*test)
  {
    p = 2 * M_PI * sqrt(out.m / out.k);
    fprintf(stderr, "Periodo: %lf. Delta t consigliato: tra %lf e %lf.\n", p, p / 20, p / 100);
    fprintf(stderr, "dt: ");
    scanf("%lf", dt);
    fprintf(stderr, "tmax: ");
    scanf("%lf", tmax);
  }
  return out;
} /*getData*/

double accel(double x, param initParam)
{
  return -initParam.k * x / initParam.m;
} /*accel*/

void eulercrom(param initParam, double dt, double tmax, posTime *XandT)
{
  double x = initParam.x0, v = initParam.v0, dE, E0;
  int i, N;
  printf("#Algoritmo di Eulero-Cromer \n#K=%lf m=%lf x0=%lf v0=%lf dt=%lf \n#Colonne: \n#1:t\t2:x\t3:v\t4:(E-E0)/E0 \n", initParam.k, initParam.m, initParam.x0, initParam.v0, dt);
  N = (int)(tmax / dt);
  E0 = v * v * initParam.m / 2 - initParam.k * x * x / 2;
  for (i = 0; i < N + 1; i++)
  {
    v = v + accel(x, initParam) * dt;
    x = x + v * dt;
    dE = (v * v * initParam.m / 2 - initParam.k * x * x / 2 - E0) / E0;
    printf("%lf\t%lf\t%lf\t%lf\n", i * dt, x, v, dE);
    (*(XandT + i)).x = x;
    (*(XandT + i)).t = i * dt;
  }
  printf("\n");
} /*eulercrom*/

void centralPoint(param initParam, double dt, double tmax, posTime *XandT)
{
  double x = initParam.x0, v1 = initParam.v0, v2, dE, E0;
  int i, N;
  printf("#Algoritmo Central Point \n#K=%lf m=%lf x0=%lf v0=%lf dt=%lf \n#Colonne: \n#1:t\t2:x\t3:v\t4:(E-E0)/E0 \n", initParam.k, initParam.m, initParam.x0, initParam.v0, dt);
  N = (int)(tmax / dt);
  E0 = v1 * v1 * initParam.m / 2 - initParam.k * x * x / 2;
  for (i = 0; i < N + 1; i++)
  {
    v2 = v1 + accel(x, initParam) * dt;
    x = x + (v2 + v1) * dt / 2;
    dE = (v2 * v2 * initParam.m / 2 - initParam.k * x * x / 2 - E0) / E0;
    printf("%lf\t%lf\t%lf\t%lf\n", i * dt, x, v2, dE);
    v1 = v2;
    (*(XandT + i)).x = x;
    (*(XandT + i)).t = i * dt;
  }
  printf("\n");
} /*cpoint*/

void velVerlet(param initParam, double dt, double tmax, posTime *XandT)
{
  double x1 = initParam.x0, x2, v = initParam.v0, dE, E0;
  int i, N;
  printf("#Algoritmo Leap Frog \n#K=%lf m=%lf x0=%lf v0=%lf dt=%lf \n#Colonne: \n#1:t\t2:x\t3:v\t4:(E-E0)/E0 \n", initParam.k, initParam.m, initParam.x0, initParam.v0, dt);
  N = (int)(tmax / dt);
  E0 = v * v * initParam.m / 2 - initParam.k * x1 * x1 / 2;
  for (i = 0; i < N + 1; i++)
  {
    x2 = x1 + v * dt + accel(x1, initParam) * dt * dt / 2;
    v = v + (accel(x2, initParam) + accel(x1, initParam)) * dt / 2;
    dE = (v * v * initParam.m / 2 - initParam.k * x2 * x2 / 2 - E0) / E0;
    printf("%lf\t%lf\t%lf\t%lf\n", i * dt, x2, v, dE);
    x1 = x2;
    (*(XandT + i)).x = x2;
    (*(XandT + i)).t = i * dt;
  }
  printf("\n");
} /*verlet*/

void rungekutta4(param initParam, double dt, double tmax, posTime *XandT)
{
  double x1 = initParam.x0, x2, v = initParam.v0, dE, E0;
  double k1, k2, k3, k4, l1, l2, l3, l4;
  int i, N;
  printf("#Algoritmo Runge Kutta 4 \n#K=%lf m=%lf x0=%lf v0=%lf dt=%lf \n#Colonne: \n#1:t\t2:x\t3:v\t4:(E-E0)/E0 \n", initParam.k, initParam.m, initParam.x0, initParam.v0, dt);
  N = (int)(tmax / dt);
  E0 = v * v * initParam.m / 2 - initParam.k * x1 * x1 / 2;
  for (i = 0; i < N + 1; i++)
  {
    k1 = v;
    l1 = accel(x1, initParam);
    k2 = v + l1 * dt / 2;
    l2 = accel(x1 + k1 * dt / 2, initParam);
    k3 = v + l2 * dt / 2;
    l3 = accel(x1 + k2 * dt / 2, initParam);
    k4 = v + l3 * dt;
    l4 = accel(x1 + k3 * dt, initParam);
    x2 = x1 + (k1 + 2 * (k2 + k3) + k4) * dt / 6;
    v = v + (l1 + 2 * (l2 + l3) + l4) * dt / 6;
    dE = (v * v * initParam.m / 2 - initParam.k * x2 * x2 / 2 - E0) / E0;
    printf("%lf\t%lf\t%lf\t%lf\n", i * dt, x2, v, dE);
    x1 = x2;
    (*(XandT + i)).x = x2;
    (*(XandT + i)).t = i * dt;
  }
  printf("\n");
} /*RUNGEKUTTA4*/
