#include <stdio.h>
#include <stdlib.h>
#define N 100

void init(double[N][N], double *, int);
void toTriangular(double[N][N], double *, int);
void solve(double[N][N], double *, double *, int);
void printArray(double *, int);

int main() {
  /*Solve equation systems using the Gauss method*/

  double A[N][N], b[N], x[N];
  double C, S;
  int n, i, j, k;

  printf("Enter the number of equations (up to 100): ");
  scanf("%d", &n);

  printf("Enter the coefficients\n");
  init(A, b, n);

  toTriangular(A, b, n);
  solve(A, b, x, n);
  printArray(x, n);

  return 0;
}

void init(double A[N][N], double *b, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("A[%d,%d] = ", i, j);
      scanf("%lf", &A[i][j]);
    }
    printf("b[%d] = ", i);
    scanf("%lf", &b[i]);
  }
}

void toTriangular(double A[N][N], double *b, int n) {
  for (int i = 0; i < n; i++) {
    // divide equations for diagonal element C
    double C = A[i][i];
    b[i] /= C;
    for (int j = 0; j < n; j++)
      A[i][j] /= C;

    // subtract the normalized equation to the other ones
    for (int k = i + 1; k < n; k++) {
      C = A[k][i];
      b[k] -= C * b[i];
      for (int j = i; j < n; j++)
        A[k][j] -= A[i][j] * C;
    }
  }
}

void solve(double A[N][N], double *b, double *x, int n) {
  for (int k = n - 1; k >= 0; k--) {
    double S = 0.;
    for (int i = k + 1; i < n; i++)
      S += A[k][i] * x[i];
    x[k] = b[k] - S;
  }
}

void printArray(double *x, int n) {
  for (int i = 0; i < n; i++)
    printf("x[%d]=%lf\n", i, x[i]);
}