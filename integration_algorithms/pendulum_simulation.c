#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define SEMPLICE 0
#define SMORZATO 1
#define FORZATO 2
#define DOPPIO 3
#define RIPSMORZ 1
#define NORIPSMORZ 0
#define BACINO 1
#define SEZIONE 0
#define NULLTEST 2

/*definizone tipi*/
typedef struct cinematica
{
  double x, v, t; /*posizione e velocita angolari*/
} cinematica;
typedef struct parametri
{
  float g, r, xP, vP, x2P, v2P, cSmorzante, ampForzante, frForzante;
} parametri;

/*variabili globali*/
parametri inPar;
int modo;
double (*accelerazione)(cinematica);

/*prototipi delle funzioni*/
cinematica rungeKutta4(cinematica, float);
void rungeKutta4Doppio(cinematica *, cinematica *, float);
double accelSmorz(cinematica);
double accelSempl(cinematica);
double accelForz(cinematica);
double accelDoppio(cinematica, cinematica);
double accelDoppio2(cinematica, cinematica);
void getData(float *, int *);
double energia(cinematica);
double energiaDoppio(cinematica, cinematica);
void sceltaModo();
void acquisizioneParametri();
int acquisizioneTempi(float *, int *);
int controlloZero(double, double, int *);

/*main*/
int main()
{
  cinematica pendolo, pendolo2, check;
  int i, numeroPassi, salto, numOsc, ripsmorz, optionForzato, tMax, nuoveCond, j;
  float dt, periodoCampionamento;
  double E, E0, tZero1, tZero2;
  long double erroreEnergia;
  FILE *fperiod, *fBacNeg, *fBacPos, *fSez;

  /*inizializzo il programma: acquisisco parametri, modalita' e apro i canali*/
  if ((fperiod = fopen("semiper.dat", "w")) == NULL)
  {
    fprintf(stderr, "ERRORE: impossibile aprire 'semiper.dat'.\n");
    exit(EXIT_FAILURE);
  }
  sceltaModo();
  acquisizioneParametri();
  numeroPassi = acquisizioneTempi(&dt, &tMax);
  if (modo == FORZATO)
  {
    fprintf(stderr, "Cosa studiare?\n%d)Sezioni di Poincare'\n%d)Bacini di attrazione\n%d)Nulla\n: ", SEZIONE, BACINO, NULLTEST);
    scanf("%d", &optionForzato);
  }
  if (optionForzato == BACINO)
  {
    if ((fBacPos = fopen("bacpos.dat", "w")) == NULL)
    {
      fprintf(stderr, "Impossibile aprire 'bacpos.dat'\n");
      exit(EXIT_FAILURE);
    }
    if ((fBacNeg = fopen("bacneg.dat", "w")) == NULL)
    {
      fprintf(stderr, "Impossibile aprire 'bacneg.dat'\n");
      exit(EXIT_FAILURE);
    }
    fprintf(fBacPos, "#Bacino di attrazione a velocita' positive\n#Parametri: t=%d f=%f\n#pos\tvel\n", tMax, inPar.ampForzante);
    fprintf(fBacNeg, "#Bacino di attrazione a velocita' negative\n#Parametri: t=%d f=%f\n#pos\tvel\n", tMax, inPar.ampForzante);
  }
  if (optionForzato == SEZIONE)
  {
    if ((fSez = fopen("sezione.dat", "w")) == NULL)
    {
      fprintf(stderr, "Impossibile aprire 'sezione.dat'\n");
      exit(EXIT_FAILURE);
    }
    periodoCampionamento = 2 * M_PI / inPar.frForzante;
    fprintf(fSez, "#Sezione di Poincare' a f=%f e periodo della forzante pari a %f\n#pos\t\tvel\n", inPar.ampForzante, periodoCampionamento);
  }
  /*fine inizializzazione*/

  /*corpo del programma*/
  do
  {
    if (ripsmorz == RIPSMORZ)
      inPar.cSmorzante += 0.01; /*per cercare il coefficiente di smorzamento critico*/
    ripsmorz = NORIPSMORZ;
    pendolo.x = inPar.xP;
    pendolo.v = inPar.vP;
    if (modo == DOPPIO)
    { /*energia iniziale*/
      pendolo2.x = inPar.x2P;
      pendolo2.v = inPar.v2P;
      E0 = energiaDoppio(pendolo, pendolo2);
      printf("#t\t\tpos1\t\tpos2\t\tvel1\t\tvel2\t\tdE/E0\n");
    }
    else
    {
      E0 = energia(pendolo);
      printf("#t\t\tpos\t\tvel\t\tdE/E0\n");
    }
    tZero1 = 0;
    salto = 1;
    numOsc = 0;
    j = 1;
    for (i = 0; i < numeroPassi + 1; i++)
    { /*integrazione*/
      pendolo.t = i * dt;
      if (modo == DOPPIO)
      { /*per pendolo doppio*/
        pendolo2.t = i * dt;
        E = energiaDoppio(pendolo, pendolo2);
        erroreEnergia = (long double)(E - E0) / E0;
        printf("%lf\t%lf\t%lf\t%lf\t%lf\t%Lf\n", pendolo.t, pendolo.x, pendolo2.x, pendolo.v, pendolo2.v, erroreEnergia);
        rungeKutta4Doppio(&pendolo, &pendolo2, dt);
      }
      else
      { /*per pendoli non-doppi*/
        check = pendolo;
        E = energia(pendolo);
        erroreEnergia = (long double)(E - E0) / E0;
        printf("%lf\t%lf\t%lf\t%Lf\n", pendolo.t, pendolo.x, pendolo.v, erroreEnergia);
        pendolo = rungeKutta4(pendolo, dt);
        /*campionamento per la sezione di poincare'*/
        if (optionForzato == SEZIONE && (periodoCampionamento * j) >= pendolo.t && (periodoCampionamento * j) < ((i + 1) * dt))
        {
          fprintf(fSez, "%lf\t%lf\n", pendolo.x, pendolo.v);
          j++;
        }
        if (salto == 0)
        { /*per cercare i semiperiodi*/
          if (controlloZero(check.x, pendolo.x, &salto))
          {
            tZero2 = check.t - check.x * dt / (pendolo.x - check.x);
            if (tZero1)
              fprintf(fperiod, "%lf\n", tZero2 - tZero1);
            tZero1 = tZero2;
            if (modo == SMORZATO)
              numOsc++;
          }
        }
        else
        { /*salto per evitare di prendere due volte lo stesso zero*/
          salto = 0;
        }
      }
    }
    /*bacini di attrazione per pendolo forzato*/
    if (optionForzato == BACINO)
    {
      if (pendolo.v < 0)
        fprintf(fBacNeg, "%lf\t%lf\n", inPar.xP, inPar.vP);
      else
        fprintf(fBacPos, "%lf\t%lf\n", inPar.xP, inPar.vP);
      fprintf(stderr, "Inserire nuove condizioni iniziali?(0=n;1=y): ");
      scanf("%d", &nuoveCond);
      if (nuoveCond)
      {
        fprintf(stderr, "Inserire posizione iniziale: ");
        scanf("%f", &(inPar.xP));
        fprintf(stderr, "Inserire velocita' iniziale: ");
        scanf("%f", &(inPar.vP));
      }
    }
    /*per la ricerca del coefficiente di smorzamento critico*/
    if (modo == SMORZATO)
    {
      fprintf(stderr, "Con coefficiente di smorzamento %.2lf, il pendolo compie %d oscillazioni.\nRipetere incrementando il coefficiente di 0.01? (0=n,1=y):", inPar.cSmorzante, numOsc);
      scanf("%d", &ripsmorz);
    }
  } while ((modo == SMORZATO && ripsmorz == RIPSMORZ) || (optionForzato == BACINO && nuoveCond));
  fclose(fperiod);
  if (optionForzato == BACINO)
  {
    fclose(fBacNeg);
    fclose(fBacPos);
  }
  if (optionForzato == SEZIONE)
  {
    fclose(fSez);
  }
  return 0;
}

int controlloZero(double in, double in2, int *salto)
{
  double prod;
  prod = in * in2;
  if (prod <= 0)
  {
    if (prod == 0)
      *salto = 1;
    else
      *salto = 0;
    return 1;
  }
  else
    return 0;
}

double energiaDoppio(cinematica in, cinematica in2)
{ /*nel calcolo dell'errore sull'energia è irrilevante la massa, che si semplifica*/
  double V, K;
  K = 0.5 * inPar.r * inPar.r * (2 * in.v * in.v + in2.v * in2.v + 2 * in.v * in2.v * cos(in.x - in2.x));
  V = -inPar.r * inPar.g * (2 * cos(in.x) + cos(in2.x));
  return V + K;
}

double energia(cinematica in)
{ /*nel calcolo dell'errore sull'energia è irrilevante la massa, che si semplifica*/
  double V, K;
  K = 0.5 * inPar.r * inPar.r * in.v * in.v;
  V = inPar.g * inPar.r * (1 - cos(in.x));
  return V + K;
}

int acquisizioneTempi(float *dt, int *tMax)
{
  do
  {
    fprintf(stderr, "Tempo massimo d'integrazione: ");
    scanf("%d", tMax);
  } while (tMax <= 0);
  do
  {
    fprintf(stderr, "Passo d'integrazione, dt: ");
    scanf("%f", dt);
  } while ((*dt) <= 0);
  return (int)((*tMax) / (*dt)) + 1;
}

void sceltaModo()
{
  do
  {
    fprintf(stderr, "%d)Pendolo semplice;\n%d)pendolo smorzato;\n%d)pendolo smorzato sottoposto a forzante periodica;\n%d)pendolo doppio.\n:", SEMPLICE, SMORZATO, FORZATO, DOPPIO);
    scanf("%d", &modo);
  } while (modo != DOPPIO && modo != SEMPLICE && modo != SMORZATO && modo != FORZATO);
  if (modo == SEMPLICE)
    accelerazione = &accelSempl;
  else if (modo == FORZATO)
    accelerazione = &accelForz;
  else if (modo == SMORZATO)
    accelerazione = &accelSmorz;
}

void acquisizioneParametri()
{
  do
  {
    fprintf(stderr, "Accelerazione gravitazionale: ");
    scanf("%f", &(inPar.g));
  } while (inPar.g <= 0);
  do
  {
    fprintf(stderr, "Lunghezza asta/e: ");
    scanf("%f", &(inPar.r));
  } while (inPar.r <= 0);
  inPar.cSmorzante = inPar.ampForzante = 0;
  if (modo != SEMPLICE && modo != DOPPIO)
  {
    do
    {
      fprintf(stderr, "Coefficiente d'attrito viscoso: ");
      scanf("%f", &(inPar.cSmorzante));
    } while (inPar.cSmorzante <= 0);
    if (modo == FORZATO)
    {
      do
      {
        fprintf(stderr, "Ampiezza della forzante: ");
        scanf("%f", &(inPar.ampForzante));
      } while (inPar.ampForzante <= 0);
      do
      {
        fprintf(stderr, "Frequenza della forzante: ");
        scanf("%f", &(inPar.frForzante));
      } while (inPar.frForzante <= 0);
    }
  }
  if (modo == DOPPIO)
  {
    fprintf(stderr, "Posizione iniziale (rad) del primo punto: ");
    scanf("%f", &(inPar.xP));
    fprintf(stderr, "Velocita' iniziale (rad/s) del primo punto: ");
    scanf("%f", &(inPar.vP));
    fprintf(stderr, "Posizione iniziale (rad) del secondo punto: ");
    scanf("%f", &(inPar.x2P));
    fprintf(stderr, "Velocita' iniziale (rad/s) del secondo punto: ");
    scanf("%f", &(inPar.v2P));
  }
  else
  {
    fprintf(stderr, "Posizione iniziale (rad): ");
    scanf("%f", &(inPar.xP));
    fprintf(stderr, "Velocita' iniziale (rad/s): ");
    scanf("%f", &(inPar.vP));
  }
}

/*rk4*/
cinematica rungeKutta4(cinematica in, float dt)
{
  cinematica temp, out;
  double l[4], k[4], dtMezzi;
  dtMezzi = dt * 0.5;
  temp.t = in.t;
  k[0] = in.v;
  l[0] = accelerazione(in);
  k[1] = in.v + l[0] * dtMezzi;
  temp.x = in.x + k[0] * dtMezzi;
  temp.v = k[1];
  l[1] = accelerazione(temp);
  k[2] = in.v + l[1] * dtMezzi;
  temp.x = in.x + k[1] * dtMezzi;
  temp.v = k[2];
  l[2] = accelerazione(temp);
  k[3] = in.v + l[2] * dt;
  temp.x = in.x + k[2] * dt;
  temp.v = k[3];
  l[3] = accelerazione(temp);
  out.x = in.x + (k[0] + 2 * k[1] + 2 * k[2] + k[3]) * dt / 6;
  out.v = in.v + (l[0] + 2 * l[1] + 2 * l[2] + l[3]) * dt / 6;
  return out;
}

/*rk4 pendolo doppio*/
void rungeKutta4Doppio(cinematica *pend, cinematica *pend2, float dt)
{
  cinematica temp, temp2;
  double l[4], k[4], l2[4], k2[4], dtMezzi;
  dtMezzi = dt * 0.5;
  temp.t = pend->t;
  temp2.t = pend2->t;
  temp.x = pend->x;
  temp.v = pend->v;
  temp2.x = pend2->x;
  temp2.v = pend2->v;
  k[0] = pend->v;
  k2[0] = pend2->v;
  l[0] = accelDoppio(temp, temp2);
  l2[0] = accelDoppio2(temp, temp2);
  k[1] = pend->v + l[0] * dtMezzi;
  k2[1] = pend2->v + l2[0] * dtMezzi;
  temp.x = pend->x + k[0] * dtMezzi;
  temp.v = k[1];
  temp2.x = pend2->x + k2[0] * dtMezzi;
  temp2.v = k2[1];
  l[1] = accelDoppio(temp, temp2);
  l2[1] = accelDoppio2(temp, temp2);
  k[2] = pend->v + l[1] * dtMezzi;
  k2[2] = pend2->v + l2[1] * dtMezzi;
  temp.x = pend->x + k[1] * dtMezzi;
  temp.v = k[2];
  temp2.x = pend2->x + k[1] * dtMezzi;
  temp2.v = k2[2];
  l[2] = accelDoppio(temp, temp2);
  l2[2] = accelDoppio2(temp, temp2);
  k[3] = pend->v + l[2] * dt;
  k2[3] = pend2->v + l2[2] * dt;
  temp.x = pend->x + k[2] * dt;
  temp.v = k[3];
  temp2.x = pend2->x + k2[2] * dt;
  temp2.v = k2[3];
  l[3] = accelDoppio(temp, temp2);
  l2[3] = accelDoppio2(temp, temp2);
  pend->x += (k[0] + 2 * k[1] + 2 * k[2] + k[3]) * dt / 6;
  pend->v += (l[0] + 2 * l[1] + 2 * l[2] + l[3]) * dt / 6;
  pend2->x += (k2[0] + 2 * k2[1] + 2 * k2[2] + k2[3]) * dt / 6;
  pend2->v += (l2[0] + 2 * l2[1] + 2 * l2[2] + l2[3]) * dt / 6;
}

double accelSempl(cinematica pendolo)
{
  return -(inPar.g / inPar.r) * sin(pendolo.x);
}

double accelSmorz(cinematica pendolo)
{
  return accelSempl(pendolo) - inPar.cSmorzante * pendolo.v;
}

double accelForz(cinematica pendolo)
{
  return accelSmorz(pendolo) + inPar.ampForzante * cos(inPar.frForzante * pendolo.t);
}

double accelDoppio(cinematica pendolo, cinematica pendolo2)
{
  double diff, cosDiff, num, denom;
  diff = pendolo.x - pendolo2.x;
  cosDiff = cos(diff);
  num = sin(diff) * (pendolo.v * pendolo.v * cosDiff + pendolo2.v * pendolo2.v) + inPar.g * (2 * sin(pendolo.x) - sin(pendolo2.x) * cosDiff) / inPar.r;
  denom = cosDiff * cosDiff - 2;
  return num / denom;
}

double accelDoppio2(cinematica pendolo, cinematica pendolo2)
{
  double diff, cosDiff, num, denom;
  diff = pendolo.x - pendolo2.x;
  cosDiff = cos(diff);
  num = sin(diff) * (pendolo.v * pendolo.v + 0.5 * pendolo2.v * pendolo2.v * cosDiff) + inPar.g * (sin(pendolo.x) * cosDiff - sin(pendolo2.x)) / inPar.r;
  denom = 1 - 0.5 * cosDiff * cosDiff;
  return num / denom;
}
