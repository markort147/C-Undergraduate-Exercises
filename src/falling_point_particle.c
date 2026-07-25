#include <stdio.h>
#include <stdlib.h>
#define G (9.81)

static void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

static double read_double_min(const char* prompt, double min, int inclusive) {
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
        if ((inclusive && x >= min) || (!inclusive && x > min)) return x;
        // else loop again
    }
}

static int read_choice(const char* prompt) {
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
        if (opt == 0 || opt == 1) return opt;
    }
}

int main() {
    while (1) {
        double t = read_double_min("Enter the time (s): ", 0.0, 0);
        double m = read_double_min("Enter the mass (kg): ", 0.0, 0);
        double v_0 = read_double_min("Enter the initial speed (m/s): ", 0.0, 1);
        double h_0 = read_double_min("Enter the initial height (m): ", 0.0, 0);

        double v_t = v_0 - G * t;
        double h_t = h_0 - v_0 * t - .5 * G * t * t;

        if (h_t > 0) {
            printf("************************************************************\n");
            printf("  The particle is still %lf m far from the ground!\n", h_t);
            printf("    Potential energy: %10lf J\n", m * h_t * G);
            printf("    Kinetic energy: %lf J\n", .5 * m * v_t * v_t);
            printf("************************************************************\n");
        } else {
            printf("*********************************************\n");
            printf("  The particle reached the ground!\n");
            printf("    Kinetic energy: %lf J\n", m * h_0 * G);
            printf("*********************************************\n");
        }
        
        if (!read_choice("1 for changing time, 0 for exit: ")) break;
    }
}