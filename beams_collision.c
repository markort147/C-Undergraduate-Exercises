#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BEAM_RADIUS (1)
#define MAX_PARTICLES (6000)
#define MAX_PARTICLE_RADIUS (0.1)

typedef struct {
  double x, y;
} Particle;

static Particle *generate_coordinates(int num) {
  Particle *beam = (Particle *)malloc(num * sizeof(Particle));
  if (beam != NULL) {
    double beam_radius_sqr = BEAM_RADIUS * BEAM_RADIUS;
    for (int i = 0; i < num; i++) {
      Particle *p = beam + i;
      do {
        p->x = (2.0 * rand() / (double)RAND_MAX - 1.0) * BEAM_RADIUS;
        p->y = (2.0 * rand() / (double)RAND_MAX - 1.0) * BEAM_RADIUS;
      } while ((p->x * p->x) + (p->y * p->y) - beam_radius_sqr > 0);
    }
  }
  return beam;
}

static void clear_stdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}

int main() {

  srand(time(NULL));

  printf("This program simulates, in a simplified way, the collision between "
         "two particle beams.\n");
  printf("A single collision occurs when a particle from the first "
         "beam falls within a circle centred on a particle from the second "
         "beam, with a radius defined by the user.\n");

  // Acquiring parameters
  int scan_res;
  int num_particles_1;
  do {
    printf("Enter the amount of particles in the first beam (0,%d]:",
           MAX_PARTICLES);
    scan_res = scanf("%d", &num_particles_1);
    if (scan_res == EOF) {
      fprintf(stderr, "EOF on input\n");
      return 1;
    }
    if (scan_res != 1)
      clear_stdin();
  } while (scan_res != 1 || num_particles_1 <= 0 || num_particles_1 > MAX_PARTICLES);

  int num_particles_2;
  do {
    printf("Enter the amount of particles in the second beam (0,%d]:",
           MAX_PARTICLES);
    scan_res = scanf("%d", &num_particles_2);
    if (scan_res == EOF) {
      fprintf(stderr, "EOF on input\n");
      return 1;
    }
    if (scan_res != 1)
      clear_stdin();
  } while (scan_res != 1 || num_particles_2 <= 0 || num_particles_2 > MAX_PARTICLES);

  double particle_radius;
  do {
    printf("Enter the particles radius (0,%f]:", MAX_PARTICLE_RADIUS);
    scan_res = scanf("%lf", &particle_radius);
    if (scan_res == EOF) {
      fprintf(stderr, "EOF on input\n");
      return 1;
    }
    if (scan_res != 1)
      clear_stdin();
  } while (scan_res != 1 || particle_radius <= 0 ||
           particle_radius > MAX_PARTICLE_RADIUS);

  // Generating random coordinates
  Particle *beam1 = generate_coordinates(num_particles_1);
  if (beam1 == NULL) {
    printf("Error instantiating beam1\n");
    return 1;
  }

  Particle *beam2 = generate_coordinates(num_particles_2);
  if (beam2 == NULL) {
    printf("Error instantiating beam2\n");
    free(beam1);
    return 1;
  }

  // Counting the collisions
  int collisions = 0;
  double particle_radius_sqr = particle_radius * particle_radius;
  for (int i = 0; i < num_particles_1; i++) {
    Particle p1 = *(beam1 + i);
    for (int j = 0; j < num_particles_2; j++) {
      Particle p2 = *(beam2 + j);
      double dx = p1.x - p2.x;
      double dy = p1.y - p2.y;
      if (dx * dx + dy * dy <= particle_radius_sqr) {
        collisions++;
        break;
      }
    }
  }

  // Printing the result
  printf("Collisions: %d\n", collisions);
  printf("Ratio of collisions in the first beam: %f%%\n",
         100. * collisions / (double)num_particles_1);
  printf("Ratio of collisions in the second beam: %f%%\n",
         100. * collisions / (double)num_particles_2);

  free(beam1);
  free(beam2);
  return 0;
}