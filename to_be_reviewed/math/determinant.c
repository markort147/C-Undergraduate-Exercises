#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NR 20
#define NC 20

int determinante(int[NR][NC], int, int);
int getInt(char *, int, int);
void complementare(int[NR][NC], int, int, int[NR - 1][NC - 1]);
void fillRandMatrix(int[NR][NC], int, int);
void printMatrix(int[NR][NC], int, int);

int main() {
  int matrix[NR][NC];
  int det, nr, nc;
  srand(time(0));
  nr = getInt("Numero righe", 1, NR);
  nc = getInt("Numero colonne", 1, NC);
  fillRandMatrix(matrix, nr, nc);
  printMatrix(matrix, nr, nc);
  det = determinante(matrix, nr, nc);
  printf("Determinante: %d\n", det);
  return 0;
}

void printMatrix(int matrix[NR][NC], int nr, int nc) {
  int i, j;
  printf("MATRICE:\n");
  for (i = 0; i < nr; i++) {
    for (j = 0; j < nc; j++) {
      printf("%d\t", matrix[i][j]);
    }
    printf("\n");
  }
}

void fillRandMatrix(int matrix[NR][NC], int nr, int nc) {
  int i, j;
  for (i = 0; i < nr; i++) {
    for (j = 0; j < nc; j++) {
      matrix[i][j] = rand() % 6;
    }
  }
}

int getInt(char *mess, int min, int max) {
  int x;
  do {
    printf("%s tra %d e %d: ", mess, min, max);
    scanf("%d", &x);
  } while (x < min || x > max);
  return x;
}

int determinante(int matrix[NR][NC], int nr, int nc) {
  double det = 0;
  int compl[NR - 1][NC - 1];
  int i;
  if (nr != nc) {
    printf("La matrice non è quadrata! Ritorno 0.\n");
    return 0;
  } else if (nr == 1) {
    return matrix[0][0];
  } else if (nr == 2) {
    return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
  } else if (nr == 3) {
    return matrix[0][0] * matrix[1][1] * matrix[2][2] +
           matrix[1][0] * matrix[2][1] * matrix[0][2] +
           matrix[0][1] * matrix[1][2] * matrix[2][0] -
           matrix[2][0] * matrix[1][1] * matrix[0][2] -
           matrix[2][1] * matrix[1][2] * matrix[0][0] -
           matrix[1][0] * matrix[0][1] * matrix[2][2];
  } else {
    for (i = nr - 1; i >= 0; i--) {
      complementare(matrix, nr, i, compl);
      det +=
          pow(-1, i) * matrix[nr - 1][i] * determinante(compl, nr - 1, nr - 1);
    }
    return det;
  }
}

void complementare(int matrix[NR][NC], int ordine, int colonna,
                   int compl[NR - 1][NC - 1]) {
  int i, j;
  for (i = 0; i < ordine; i++) {
    if (i < colonna) {
      for (j = 0; j < ordine - 1; j++) {
        compl[j][i] = matrix[j][i];
      }
    } else if (i > colonna) {
      for (j = 0; j < ordine - 1; j) {
        compl[j - 1][i - 1] = matrix[j][i];
      }
    }
  }
}
