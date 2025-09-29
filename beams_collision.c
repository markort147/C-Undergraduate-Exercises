#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BEAM_RADIUS (1)
#define MAX_PARTICLES (6000)
#define MAX_PARTICLE_RADIUS (0.1)

typedef struct {
  double x, y;
} Particle;

Particle *generateCoordinates(int);
void clearStdin(void);

int main() {

  srand(time(NULL));

  printf("This program simulates, in a simplified way, the collision between "
         "two particle beams.\n");
  printf("A single collision occurs when a particle from the first "
         "beam falls within a circle centred on a particle from the second "
         "beam, with a radius defined by the user.\n");

  // Acquiring parameters
  int scanRes;
  int numParticles1;
  do {
    printf("Enter the amount of particles in the first beam (0,%d]:",
           MAX_PARTICLES);
    scanRes = scanf("%d", &numParticles1);
    if (scanRes == EOF) {
      fprintf(stderr, "EOF on input\n");
      return 1;
    }
    if (scanRes != 1)
      clearStdin();
  } while (scanRes != 1 || numParticles1 <= 0 || numParticles1 > MAX_PARTICLES);

  int numParticles2;
  do {
    printf("Enter the amount of particles in the second beam (0,%d]:",
           MAX_PARTICLES);
    scanRes = scanf("%d", &numParticles2);
    if (scanRes == EOF) {
      fprintf(stderr, "EOF on input\n");
      return 1;
    }
    if (scanRes != 1)
      clearStdin();
  } while (scanRes != 1 || numParticles2 <= 0 || numParticles2 > MAX_PARTICLES);

  double particleRadius;
  do {
    printf("Enter the particles radius (0,%f]:", MAX_PARTICLE_RADIUS);
    scanRes = scanf("%lf", &particleRadius);
    if (scanRes == EOF) {
      fprintf(stderr, "EOF on input\n");
      return 1;
    }
    if (scanRes != 1)
      clearStdin();
  } while (scanRes != 1 || particleRadius <= 0 ||
           particleRadius > MAX_PARTICLE_RADIUS);

  // Generating random coordinates
  Particle *beam1 = generateCoordinates(numParticles1);
  if (beam1 == NULL) {
    printf("Error instantiating beam1\n");
    return 1;
  }

  Particle *beam2 = generateCoordinates(numParticles2);
  if (beam2 == NULL) {
    printf("Error instantiating beam2\n");
    free(beam1);
    return 1;
  }

  // Counting the collisions
  int collisions = 0;
  double particleRadiusSqr = particleRadius * particleRadius;
  for (int i = 0; i < numParticles1; i++) {
    Particle p1 = *(beam1 + i);
    for (int j = 0; j < numParticles2; j++) {
      Particle p2 = *(beam2 + j);
      double dx = p1.x - p2.x;
      double dy = p1.y - p2.y;
      if (dx * dx + dy * dy <= particleRadiusSqr) {
        collisions++;
        break;
      }
    }
  }

  // Printing the result
  printf("Collisions: %d\n", collisions);
  printf("Ratio of collisions in the first beam: %f%%\n",
         100. * collisions / (double)numParticles1);
  printf("Ratio of collisions in the second beam: %f%%\n",
         100. * collisions / (double)numParticles2);

  free(beam1);
  free(beam2);
  return 0;
}

Particle *generateCoordinates(int num) {
  Particle *beam = (Particle *)malloc(num * sizeof(Particle));
  if (beam != NULL) {
    double beamRadiusSqr = BEAM_RADIUS * BEAM_RADIUS;
    for (int i = 0; i < num; i++) {
      Particle *p = beam + i;
      do {
        p->x = (2.0 * rand() / (double)RAND_MAX - 1.0) * BEAM_RADIUS;
        p->y = (2.0 * rand() / (double)RAND_MAX - 1.0) * BEAM_RADIUS;
      } while ((p->x * p->x) + (p->y * p->y) - beamRadiusSqr > 0);
    }
  }
  return beam;
}

void clearStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}