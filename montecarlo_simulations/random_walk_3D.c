#include <stdio.h>
#include <stdlib.h>
#define TMAX 1000
#define TRAJECTORIES 1000

typedef struct coordinates
{
  long int x, y, z;
} coords;

int main()
{
  /*seed*/
  FILE *fseed;
  unsigned int seed;
  fseed = fopen("/dev/urandom", "r");
  fread(&seed, sizeof(unsigned int), 1, fseed);
  fclose(fseed);
  srand48(seed);
  /*variables statement*/
  coords pos;
  double r, b1, b2, b3, b4, b5;
  int i, j;
  /*randwalk3D and printing*/
  b1 = 70. / 88;
  b2 = 72. / 88;
  b3 = 76. / 88;
  b4 = 80. / 88;
  b5 = 84. / 88;
  for (j = 0; j < TRAJECTORIES; j++)
  {
    pos.x = pos.y = pos.z = 0;
    for (i = 0; i < TMAX; i++)
    {
      r = drand48();
      if (r < b1)
        pos.z--;
      else if (r < b2)
        pos.z++;
      else if (r < b3)
        pos.x++;
      else if (r < b4)
        pos.x--;
      else if (r < b5)
        pos.y++;
      else
        pos.y--;
      printf("%ld\t%ld\t%ld\n", pos.x, pos.y, pos.z);
    }
    printf("\n");
  }
  return 0;
}
