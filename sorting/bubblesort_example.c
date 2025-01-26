#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SIZE 100
#define ITEMS_PER_ROW 10

void printArray(int *);
void bubblesort(int *);
void randomFill(int *);

int main()
{
  srand(time(0));
  
  int array[SIZE];
  randomFill(array);
  
  printf("Random array:\n");
  printArray(array);

  printf("Press enter to continue");
  char wait;
  scanf("%c", wait);
  bubblesort(array);

  printf("Sorted array:\n");
  printArray(array);
}

void randomFill(int* array)
{
  for (int i = 0; i < SIZE; i++)
  {
    array[i] = rand();
  }
}

void printArray(int *array)
{
  for (int i = 0; i < SIZE; i += ITEMS_PER_ROW)
  {
    for (int j = 0; j < ITEMS_PER_ROW; j++)
    {
      printf("%d\t", array[i + j]);
    }
    printf("\n");
  }
}

void bubblesort(int *array)
{
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = SIZE; j > i; j--)
    {
      if (array[j - 1] > array[j])
      {
        int temp = array[j];
        array[j] = array[j - 1];
        array[j - 1] = temp;
      }
    }
  }
}
