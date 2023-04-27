#include <stdio.h>
#include <stdlib.h>

struct Matrix
{
  int rows;
  int cols;
  int **data;
};

typedef struct Matrix matrix;

matrix *load_matrix();

int main(int argc, char *argv[])
{
  matrix *mat = (struct Matrix *)malloc(sizeof(struct Matrix));
  mat->rows = 2;
  mat->cols = 7;
  int **mat_data = (int **)malloc((mat->rows) * sizeof(int *));
  for (int row = 0; row < (mat->rows); ++row)
  {
    int *r = (int *)calloc(mat->cols, sizeof(int));
    mat_data[row] = r;
  }
  mat->data = mat_data;
  mat_data = NULL;
  printf("rows: %d, cols: %d\n", mat->rows, mat->cols);
  for (int i = 0; i < (mat->rows); ++i)
  {
    for (int j = 0; j < (mat->cols); ++j)
    {
      printf("%d ", (mat->data)[i][j]);
    }
    printf("\n");
    free((mat->data)[i]);
    (mat->data)[i] = NULL;
  }
  free((mat->data));
  mat->data = NULL;
  free(mat);
  mat = NULL;
  load_matrix();
  return 0;
}

matrix *load_matrix()
{
  int num, ret;
  while ((ret = scanf("%d", &num) == 1))
  {
    printf("loaded input: %d\n", num);
  }
  printf("scnaf retturn: %d\n", scanf("%d", &num));
  printf("loaded input: %d\n", num);
  char a;
  a = getchar();
  printf("char: %c", a);
  return NULL;
}