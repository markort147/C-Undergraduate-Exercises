#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE *finput, *foutput;
  int numEvents, i, j, iMin, iMax, option, numBins;
  int *data, *freq;
  char *fileName;
  float binSize;
  fileName = argv[1];
  numEvents = atoi(argv[2]);
  if ((finput = fopen(fileName, "r")) == NULL) {
    fprintf(stderr, "Errore: non sono riuscito ad aprire '%s'.\n", argv[1]);
  }
  if ((foutput = fopen("myHistogram.dat", "w")) == NULL) {
    fprintf(stderr, "Errore: non sono riuscito ad aprire 'myHistogram.dat'.\n");
  }
  if ((data = (int *)malloc(numEvents * sizeof(int))) == NULL) {
    fprintf(
        stderr,
        "ERRORE: non sono riuscito ad allocare memoria per l'array 'data'.\n");
  }
  fprintf(stderr, "0)# di bin pari alla radice quadrata del # di eventi;\n1)# "
                  "di bin da inserire manualmente.\n: ");
  scanf("%d", &option);
  if (option == 1) {
    fprintf(stderr, "Inserire # di bin: ");
    scanf("%d", &numBins);
  } else if (option == 0) {
    numBins = (int)sqrt(numEvents);
  }
  iMin = iMax = 0;
  for (i = 0; i < numEvents; i++) {
    fscanf(finput, "%d", data + i);
    if (data[i] > data[iMax])
      iMax = i;
    if (data[i] < data[iMin])
      iMin = i;
  }
  binSize = (data[iMax] - data[iMin]) / (float)numBins;
  if ((freq = (int *)malloc(numBins * sizeof(int))) == NULL) {
    fprintf(
        stderr,
        "ERRORE: non sono riuscito ad allocare memoria per l'array 'data'.\n");
  }
  for (i = 0; i < numEvents; i++) {
    for (j = 0; j < numBins; j++) {
      if (data[i] >= data[iMin] + j * binSize &&
          data[i] < data[iMin] + (j + 1) * binSize)
        freq[j]++;
    }
  }
  fprintf(foutput, "%d\t 0\n", data[iMin]);
  for (i = 0; i < numBins; i++) {
    fprintf(foutput, "%lf\t%lf\n%lf\t%lf\n", (data[iMin] + i * binSize),
            freq[i] / (double)numEvents, (data[iMin] + (i + 1) * binSize),
            freq[i] / (double)numEvents);
  }
  fprintf(foutput, "%d\t 0\n", data[iMax]);
  fclose(finput);
  fclose(foutput);
  return 0;
}
