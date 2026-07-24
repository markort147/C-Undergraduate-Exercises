#include <stdio.h>
#include <stdlib.h>
#define G (9.81)

static void clear_stdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}

static double read_double_min(const char *prompt, double min, int inclusive) {
  while (1) {
    double x;
    int r;
    printf("%s", prompt);
    r = scanf("%lf", &x);
    if (r == EOF) {
      fprintf(stderr, "EOF on input\n");
      exit(EXIT_FAILURE);
    }
    if (r != 1) {
      clear_stdin();
      continue;
    }
    if ((inclusive && x >= min) || (!inclusive && x > min))
      return x;
    // else loop again
  }
}

static int read_choice(const char *prompt) {
  while (1) {
    int opt;
    int r;
    printf("%s", prompt);
    r = scanf("%d", &opt);
    if (r == EOF) {
      fprintf(stderr, "EOF on input\n");
      exit(EXIT_FAILURE);
    }
    if (r != 1) {
      clear_stdin();
      continue;
    }
    if (opt == 0 || opt == 1)
      return opt;
  }
}

int main() {
  while (1) {
    double t = read_double_min("Enter the time: ", 0.0, 0);
    double m = read_double_min("Enter the mass: ", 0.0, 0);
    double v0 = read_double_min("Enter the initial speed: ", 0.0, 1);
    double h0 = read_double_min("Enter the initial height: ", 0.0, 0);

    double v = v0 - G * t;
    double h = h0 - v0 * t - .5 * G * t * t;
    double kinetic = .5 * m * v * v;
    double potential = m * h * G;

    if (h > 0.0) {
      printf("Potential energy: %lf\n", kinetic);
      printf("Kinetic energy: %lf\n", potential);
    } else
      printf("The particle reached the ground!\n");

    if (!read_choice("1 for changing time, 0 for exit: "))
      break;
  }
}