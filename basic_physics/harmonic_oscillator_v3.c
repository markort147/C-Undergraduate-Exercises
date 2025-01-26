#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define EULER 0
#define VELOCUTY_VERLET 1
#define RUNGEKUTTA4 2
#define COMPARE 3

/*struct definition*/
typedef struct param {
  double k, m, x_0, v_0;
}param;
typedef struct trajectory {
  double x, v;
}trajectory;

/*function statement*/
double Euler (param,double,double);
double VelocityVerlet (param,double,double);
double RungeKutta4 (param,double,double);
double acceleration(param,double);

int main () {
  
  /*variables statement*/
  int option, i;
  param inPar;
  double dT, Tmax, ener[3], period;
  FILE *fp;
  
  /*choice algorithm*/
  do {
    fprintf(stderr,"Choice algorithm: \n0.Euler \n1.Velocity Verlet \n2.Runge Kutta 4 \n3.Compare the algorithms \n:");
    scanf("%d",&option);
  }while(option>3);
  if(option==3) { 
    if( (fp=fopen("EvsT.dat","w")) == NULL ) {
      fprintf(stderr,"ERROR: cannot open ''EvsT.dat''\n");
      exit(EXIT_FAILURE);
    }else{
      fprintf(fp,"Comparing Euler, Velocity Verlet and Runge Kutta 4. \n\tdT\tEuler\tVelVerlet\tRK4\n");
      i=610;
    }
  }

  /*get data*/
  fprintf(stderr,"k: ");
  scanf("%lf",&inPar.k);
  fprintf(stderr,"m: ");
  scanf("%lf",&inPar.m);
  fprintf(stderr,"x_0: ");
  scanf("%lf",&inPar.x_0);
  fprintf(stderr,"v_0: ");
  scanf("%lf",&inPar.v_0);
  do{
    if(option==0 || option==3) ener[0] = Euler(inPar,dT,Tmax);
    if(option==1 || option==3) ener[1] = VelocityVerlet(inPar,dT,Tmax);
    if(option==2 || option==3) ener[2] = RungeKutta4(inPar,dT,Tmax);
    if(option==3) {
      fprintf(fp,"\t%lf\t%lf\t%lf\n",ener[0],ener[1],ener[2]);
      i-=20;
      dT=period/i;
    }
  }while(option==3 && (dT*10)<period);
  if(option==3) fclose(fp);
  return 0;
}
