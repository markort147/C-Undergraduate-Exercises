/*Dynamics of a planet under two static stars' gravitational influence.
Star A: (-DELTA/2,0).
Star B: (DELTA/2,0).
Algorithm: Velocity Verlet (2nd order).
Astronomical measurement units.*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415
#endif
#define MIN_DIST 0.005
#define DELTA 3.
#define K -4. * M_PI *M_PI
#define TEST 1
#define NTEST 0
#define dMIN 0.00001
#define dMAX 1
#define dVAR 0.00005

/*struct statement*/
typedef struct coordinates
{
  double x, y;
} coords; /*for pos, vel, acc*/
typedef struct stars
{
  double a, b;
} stars; /*for mass, dist*/

/*function statement*/
double simulation(stars, coords, coords, int, double, int, int);
stars dist(coords);
double ener(coords, stars, stars);
coords accel(coords, stars, stars);
double separation(coords, coords, stars, stars);
void getData(stars *, coords *, coords *, int *, double *, int *, int);

/*main*/
int main()
{
  stars mass;
  coords pos, vel;
  int tMax, fr, option;
  double dt, energyError;
  fprintf(stderr, "Test energy error, dE/E0, vs. dt? 1=y,0=n: ");
  scanf("%d", &option);
  if (option)
  {
    getData(&mass, &pos, &vel, &tMax, &dt, &fr, TEST);
    printf("#ENERGY ERROR TEST. \n#\t dt \t dE/E_0 \n");
    dt = dMIN;
    while (dt <= dMAX)
    {
      energyError = simulation(mass, pos, vel, tMax, dt, fr, TEST);
      printf("\t %lf \t %lf \n", dt, energyError);
      dt += dVAR;
    }
  }
  else
  {
    getData(&mass, &pos, &vel, &tMax, &dt, &fr, NTEST);
    simulation(mass, pos, vel, tMax, dt, fr, NTEST);
  }
  return 0;
}

/*getting data*/
void getData(stars *mass, coords *pos, coords *vel, int *tMax, double *dt, int *fr, int test)
{
  int option;
  fprintf(stderr, "Choice option: \n0) M_A=0, M_B=1, x_0=2.9, y_0=0, v_x=0, v_y=2*PI; \n1) M_A=1, M_B=1, x_0=2.9, y_0=0, v_x=0, v_y=2*PI; \n2) M_A=0, M_B=1, x_0=1.5, y_0=1, v_x=-2*PI, v_y=0; \n3) M_A=1, M_B=1, x_0=1.5, y_0=1, v_x=-2*PI, v_y=0; \n4) M_A=1, M_B=1, x_0=2.5, y_0=0, v_x=0, v_y=2*PI; \n5) manual; \n: ");
  scanf("%d", &option);
  if (option == 0)
  {
    mass->a = 0;
    mass->b = 1;
    pos->x = 2.9;
    pos->y = 0;
    vel->x = 0;
    vel->y = 2 * M_PI;
  }
  if (option == 1)
  {
    mass->a = 1;
    mass->b = 1;
    pos->x = 2.9;
    pos->y = 0;
    vel->x = 0;
    vel->y = 2 * M_PI;
  }
  if (option == 2)
  {
    mass->a = 0;
    mass->b = 1;
    pos->x = 1.5;
    pos->y = 1;
    vel->x = -2 * M_PI;
    vel->y = 0;
  }
  if (option == 3)
  {
    mass->a = 1;
    mass->b = 1;
    pos->x = 1.5;
    pos->y = 1;
    vel->x = -2 * M_PI;
    vel->y = 0;
  }
  if (option == 4)
  {
    mass->a = 1;
    mass->b = 1;
    pos->x = 2.5;
    pos->y = 0;
    vel->x = 0;
    vel->y = 2 * M_PI;
  }
  if (option == 5)
  {
    fprintf(stderr, "M_A: ");
    scanf("%lf", &(mass->a));
    fprintf(stderr, "M_B: ");
    scanf("%lf", &(mass->b));
    fprintf(stderr, "x_0: ");
    scanf("%lf", &(pos->x));
    fprintf(stderr, "y_0: ");
    scanf("%lf", &(pos->y));
    fprintf(stderr, "v_x: ");
    scanf("%lf", &(vel->x));
    fprintf(stderr, "v_y: ");
    scanf("%lf", &(vel->y));
  }
  fprintf(stderr, "Max time: ");
  scanf("%d", tMax);
  if (test == NTEST)
  {
    fprintf(stderr, "Time step: ");
    scanf("%lf", dt);
    fprintf(stderr, "Gnuplot should plot N=%Ld points. What fraction N/n would you plot? Insert n: ", (long long int)((*tMax) / (*dt) + 0.5));
    scanf("%d", fr);
  }
}

/*simulation with velverlet*/
double simulation(stars mass, coords pos0, coords vel, int tMax, double dt, int fr, int test)
{
  int i, j;
  stars r;
  coords pos1, acc0, acc1;
  double E_0, dE, dSep, sep0;
  r = dist(pos0);
  E_0 = ener(vel, r, mass);
  sep0 = separation(pos0, vel, r, mass);
  if (test == NTEST)
    printf("#Start simulation: \n#\t\t t \t\t x \t\t y \t\t v_x \t\t v_y \t\t r_ap \t\t r_bp \t\t dE/E0 \t dSep/sep0 \n");
  i = j = 1;
  while (r.a > MIN_DIST && r.b > MIN_DIST && (dt * i) < tMax)
  {
    acc0 = accel(pos0, r, mass);
    pos1.x = pos0.x + vel.x * dt + acc0.x * dt * dt * 0.5;
    pos1.y = pos0.y + vel.y * dt + acc0.y * dt * dt * 0.5;
    r = dist(pos1);
    acc1 = accel(pos1, r, mass);
    vel.x = vel.x + (acc1.x + acc0.x) * dt * 0.5;
    vel.y = vel.y + (acc1.y + acc0.y) * dt * 0.5;
    pos0 = pos1;
    acc0 = acc1;
    dE = ener(vel, r, mass) - E_0;
    dSep = separation(pos1, vel, r, mass) - sep0;
    if (test == NTEST)
    {
      if (j == fr)
      {
        printf("\t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf\n", dt * i, pos1.x, pos1.y, vel.x, vel.y, r.a, r.b, dE / E_0, dSep);
        j = 0;
      }
      j++;
    }
    i++;
  }
  if (test == NTEST)
  {
    if (r.a <= MIN_DIST)
      fprintf(stderr, "Stars A annihilated the planet after %lf astronomical units of time.\n", dt * (i - 1));
    else if (r.b <= MIN_DIST)
      fprintf(stderr, "Stars B annihilated the planet after %lf astronomical units of time.\n", dt * (i - 1));
  }
  return dE / E_0;
}

/*energy*/
double ener(coords vel, stars r, stars mass)
{
  double kin, pot;
  stars ratio;
  kin = 0.5 * (vel.x * vel.x + vel.y * vel.y);
  ratio.a = (double)(mass.a) / r.a;
  ratio.b = (double)(mass.b) / r.b;
  pot = K * (ratio.a + ratio.b);
  return kin + pot;
}

/*distances*/
stars dist(coords pos)
{
  stars r;
  r.a = sqrt(pos.x * pos.x + pos.y * pos.y + pos.x * DELTA + DELTA * DELTA * 0.25);
  r.b = sqrt(pos.x * pos.x + pos.y * pos.y - pos.x * DELTA + DELTA * DELTA * 0.25);
  return r;
}

/*acceleration*/
coords accel(coords pos, stars r, stars mass)
{
  coords acc;
  stars ratio;
  ratio.a = (double)(mass.a) / pow(r.a, 3);
  ratio.b = (double)(mass.b) / pow(r.b, 3);
  acc.x = K * (ratio.a + ratio.b) * pos.x + K * 0.5 * DELTA * (ratio.a - ratio.b);
  acc.y = K * (ratio.a + ratio.b) * pos.y;
  return acc;
}

/*separation constant*/
double separation(coords pos, coords vel, stars r, stars mass)
{
  return pow((pos.x * vel.y - pos.y * vel.x), 2) + vel.x * vel.x * DELTA * DELTA * 0.25 - K * pos.x * DELTA * (mass.a / r.a - mass.b / r.b);
}
