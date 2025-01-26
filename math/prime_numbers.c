#include <stdlib.h>
#include <stdio.h>

int main()
{
  int j, n;
  printf("Find prime numbers.\nEnter upper bound: ");
  scanf("%d", &n);
  for (int i = 2; i <= n; i++) {
        int isPrime = 1;
        for (j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                isPrime = 0;
                break;
            }
        }
        if (isPrime) {
            printf("%d ", i);
        }
    }
}
