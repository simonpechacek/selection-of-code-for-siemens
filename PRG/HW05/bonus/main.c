#include <stdio.h>
#include <stdlib.h>

#define NO_ERROR 0
#define INPUT_ERROR 100
#define INIT_SIZE 8
#define MEMMORY_ERROR -1
#define LETTERS_IN_ALPHABET 26
#define NUM_OPERATIONS 3
#define ADD 0
#define SUBSTRACT 1
#define MULTIPLY 2

const char NEW_MATRIX = '#';

struct Matrix
{
  char name;
  int rows;
  int cols;
  int **data;
};

typedef struct Matrix Matrix;

// USED FOR WORKING WITH MEMMORY
void check_first_row(int **row, int nums_in_row, int *row_size);
void realloc_2D_array(int ***data, int num_rows, int *data_size);
void clean_up(int **data, int num_rows);
void free_matrix_array(Matrix **matrix_array);
void free_matrix(Matrix *matrix);
void free_matrix_special(Matrix *matrix);

// LOADING MATRIXES
Matrix *load_matrix(int *err, char *matrix_name);
int **load_matrix_data(int *err, int *rows, int *cols);
int load_all_matrixes(Matrix **matrix_array);
int **create_2D_arr(int rows, int cols);

// CALCULATING MATRIXES
void print_matrix(Matrix *matrix);
int first_load(Matrix **matrix_buffer, int *operations, Matrix **matrix_array);
int solve(Matrix **matrix_array);
void left_shift(Matrix **matrix_buffer, int *operations);
int load_more(Matrix **matrix_buffer, int *operations, Matrix **matrix_array);
int two_mat_solve(Matrix **matrix_buffer, int *operations);
int advanced_solve(Matrix **matrix_buffer, int *operations, Matrix **matrix_array);

// MATRIX OPERATIONS
Matrix *add(Matrix *A, Matrix *B);
Matrix *substract(Matrix *A, Matrix *B);
Matrix *multiply(Matrix *A, Matrix *B);

// MATRIX OPERATIONS ARRAY
Matrix *(*matrix_operations[3])(Matrix *, Matrix *) = {add, substract, multiply};

const char operation_types[] = {'+', '-', '*'};

int main(int argc, char *argv[])
{
  int err = NO_ERROR;
  Matrix *matrix_array[LETTERS_IN_ALPHABET];
  load_all_matrixes(matrix_array);
  err = solve(matrix_array);
  if (err == INPUT_ERROR)
  {
    fprintf(stderr, "Error: Chybny vstup!\n");
  }
  free_matrix_array(matrix_array);
  return err;
}

int solve(Matrix **matrix_array)
{
  // Loads first 3 matrixes -> passes them to advanced solve
  // only 2 matrixes are returned the final computation is in tow_mat_solve
  Matrix *matrix_buffer[3] = {NULL, NULL, NULL};
  int operations[2] = {-1, -1};
  int err;
  err = first_load(matrix_buffer, operations, matrix_array);
  // printf("first load err %d\n", err);
  if (err != INPUT_ERROR)
  {
    if (err == 2)
    {
      err = advanced_solve(matrix_buffer, operations, matrix_array);
      // printf("advanced solve err %d\n", err);
    }
    if (err != INPUT_ERROR)
      err = two_mat_solve(matrix_buffer, operations);
    // printf("final error %d \n", err);
  }
  return err;
}

int two_mat_solve(Matrix **matrix_buffer, int *operations)
{
  // APPLY given operation on two Matricies and prints result
  int ret;
  Matrix *result = (*matrix_operations[operations[0]])(matrix_buffer[0], matrix_buffer[1]);

  if (result != NULL)
  {
    print_matrix(result);
    ret = NO_ERROR;
  }
  else
    ret = INPUT_ERROR;

  free_matrix(result);
  free_matrix_special(matrix_buffer[0]);
  free_matrix_special(matrix_buffer[1]);
  return ret;
}

int advanced_solve(Matrix **matrix_buffer, int *operations, Matrix **matrix_array)
{
  // Solves advanced matrix equation - operation order matters !!
  // keeps 3 matrixes in matrix_buffer and operations between them in operation buffer
  // solves eqations one by one in given operation order
  // after each computation loads 1 more matrix with its operation
  while (1)
  {
    if (operations[0] == MULTIPLY)
    {
      // just multiply 2 matrixes
      Matrix *result = (*matrix_operations[operations[0]])(matrix_buffer[0], matrix_buffer[1]);
      free_matrix_special(matrix_buffer[0]);
      free_matrix_special(matrix_buffer[1]);
      if (result == NULL)
      {
        free_matrix_special(matrix_buffer[2]);
        return INPUT_ERROR;
      }
      matrix_buffer[0] = result;
      left_shift(matrix_buffer, operations);
      int ret = load_more(matrix_buffer, operations, matrix_array);
      if (ret == EOF)
        return NO_ERROR;
      if (ret == INPUT_ERROR)
      {
        // printf("Load more error \n");
        free_matrix_special(matrix_buffer[0]);
        free_matrix_special(matrix_buffer[1]);
        return INPUT_ERROR;
      }
    }
    else
    {
      // ADDING OR SUBSTRACTING
      if (operations[1] != MULTIPLY)
      {
        // if next operation is addition/substarction do the first one
        Matrix *result = (*matrix_operations[operations[0]])(matrix_buffer[0], matrix_buffer[1]);
        free_matrix_special(matrix_buffer[0]);
        free_matrix_special(matrix_buffer[1]);
        if (result == NULL)
        {
          free_matrix_special(matrix_buffer[2]);
          return INPUT_ERROR;
        }
        matrix_buffer[0] = result;
        left_shift(matrix_buffer, operations);
        int ret = load_more(matrix_buffer, operations, matrix_array);
        if (ret == EOF)
          return NO_ERROR;
        if (ret == INPUT_ERROR)
        {
          // printf("Load more error \n");
          free_matrix_special(matrix_buffer[0]);
          free_matrix_special(matrix_buffer[1]);
          return INPUT_ERROR;
        }
      }
      else
      {
        // if next operation is multiplication do that
        Matrix *result = (*matrix_operations[operations[1]])(matrix_buffer[1], matrix_buffer[2]);
        free_matrix_special(matrix_buffer[1]);
        free_matrix_special(matrix_buffer[2]);
        if (result == NULL)
        {
          free_matrix_special(matrix_buffer[0]);
          return INPUT_ERROR;
        }
        matrix_buffer[1] = result;
        int ret = load_more(matrix_buffer, operations, matrix_array);
        if (ret == EOF)
          return NO_ERROR;
        if (ret == INPUT_ERROR)
        {
          // printf("Load more error \n");
          free_matrix_special(matrix_buffer[0]);
          free_matrix_special(matrix_buffer[1]);
          return INPUT_ERROR;
        }
      }
    }
  }
}

int load_more(Matrix **matrix_buffer, int *operations, Matrix **matrix_array)
{
  // Loads one matrix and operation based on equation on stdin
  // handles all error chacking
  int operation = getchar();
  int matrix_name = getchar();
  if (operation == EOF || operation == '\n')
    return EOF;
  if (matrix_name < 'A' || matrix_name > 'Z' || matrix_array[matrix_name - 'A'] == NULL)
    return INPUT_ERROR;

  switch (operation)
  {
  case '+':
    operations[1] = 0;
    break;
  case '-':
    operations[1] = 1;
    break;
  case '*':
    operations[1] = 2;
    break;
  default:
    return INPUT_ERROR;
  }
  matrix_buffer[2] = matrix_array[matrix_name - 'A'];
  return NO_ERROR;
}

void left_shift(Matrix **matrix_buffer, int *operations)
{
  // shifts matrix and operation from last place to -1 to the left
  matrix_buffer[1] = matrix_buffer[2];
  matrix_buffer[2] = NULL;
  operations[0] = operations[1];
  operations[1] = -1;
}

int first_load(Matrix **matrix_buffer, int *operations, Matrix **matrix_array)
{
  // loads first 2 - 3 matrixes checks for all errors
  // returns number of matrixes loaded or INPU_ERROR
  int num_matrixes_loaded = 0;
  int matrix_name, matrix_operation;
  for (int i = 0; i < 2; ++i)
  {
    matrix_name = getchar();
    matrix_operation = getchar();
    if (matrix_name < 'A' || matrix_name > 'Z' || matrix_array[matrix_name - 'A'] == NULL)
    {
      num_matrixes_loaded = INPUT_ERROR;
      break;
    }
    if (matrix_operation != EOF && matrix_operation != '\n' && matrix_operation != '+' && matrix_operation != '-' && matrix_operation != '*')
    {
      num_matrixes_loaded = INPUT_ERROR;
      break;
    }
    if (matrix_operation == EOF || matrix_operation == '\n')
    {
      matrix_buffer[i] = matrix_array[(matrix_name - 'A')];
      num_matrixes_loaded = i;
    }
    else
    {
      matrix_buffer[i] = matrix_array[(matrix_name - 'A')];
      for (int i = 0; i < NUM_OPERATIONS; ++i)
      {
        if (matrix_operation == operation_types[i])
        {
          matrix_operation = i;
          break;
        }
      }
      operations[i] = matrix_operation;
      num_matrixes_loaded = i;
    }
  }
  if (num_matrixes_loaded == 1 && matrix_operation != EOF && matrix_operation != '\n')
  {
    int matrix_name = getchar();
    if (matrix_name < 'A' || matrix_name > 'Z' || matrix_array[matrix_name - 'A'] == NULL)
    {
      num_matrixes_loaded = INPUT_ERROR;
    }
    else
    {
      matrix_buffer[++num_matrixes_loaded] = matrix_array[(matrix_name - 'A')];
    }
  }
  return num_matrixes_loaded;
}

int load_all_matrixes(Matrix **matrix_array)
{
  // Loads all matrixes on input
  // saves them on array based on their name
  // index to that array is: name - 'A'

  // initialize the array on NULL pointers
  for (int i = 0; i < LETTERS_IN_ALPHABET; ++i)
  {
    matrix_array[i] = NULL;
  }
  int err = NO_ERROR;
  char matrix_name;
  while (1)
  {
    Matrix *mat = load_matrix(&err, &matrix_name);

    if (err == NO_ERROR && mat != NULL)
    {
      matrix_array[matrix_name - 'A'] = mat;
    }
    else if (err == NO_ERROR && mat == NULL)
      break;
    else if (err == INPUT_ERROR)
    {
      free_matrix_array(matrix_array);
      break;
    }
  }
  return err;
}

void free_matrix_array(Matrix **matrix_array)
{
  // Frees all matrixes loaded in matrix array
  for (int i = 0; i < LETTERS_IN_ALPHABET; ++i)
  {
    free_matrix(matrix_array[i]);
  }
}

void free_matrix(Matrix *matrix)
{
  // free all dynamiclaly allocated memory used in matrix
  if (matrix != NULL)
  {
    for (int r = 0; r < (matrix->rows); ++r)
    {
      free((matrix->data)[r]);
      (matrix->data)[r] = NULL;
    }
    free(matrix->data);
    matrix->data = NULL;
    free(matrix);
  }
}

void free_matrix_special(Matrix *matrix)
{
  // free all dynamiclaly allocated memory used in matrix
  // used for newly created matrixes in computation
  // this is needed becouse 1 alloced memmory cant be freed twice

  if (matrix != NULL && matrix->name == NEW_MATRIX)
  {
    for (int r = 0; r < (matrix->rows); ++r)
    {
      free((matrix->data)[r]);
      (matrix->data)[r] = NULL;
    }
    free(matrix->data);
    matrix->data = NULL;
    free(matrix);
  }
}

void print_matrix(Matrix *matrix)
{
  // prints out matrix in given format
  putchar('[');
  for (int i = 0; i < matrix->rows; ++i)
  {
    for (int j = 0; j < matrix->cols; ++j)
    {
      if (i != 0 || j != 0)
        putchar(' ');
      printf("%d", matrix->data[i][j]);
    }
    if (i != matrix->rows - 1)
      putchar(';');
  }
  printf("]\n");
}

Matrix *load_matrix(int *err, char *matrix_name)
{
  // Loads matrix (name, rows, cols, data)

  // Read the Matrix name
  *matrix_name = getchar();
  if (*matrix_name == '\n')
  {
    *err = NO_ERROR;
    return NULL;
  }
  else if (*matrix_name < 'A' || *matrix_name > 'Z')
  {
    *err = INPUT_ERROR;
    return NULL;
  }

  Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));
  if (matrix == NULL)
  {
    fprintf(stderr, "Malloc error\n");
    exit(MEMMORY_ERROR);
  }
  int **data;
  int rows, cols;
  data = load_matrix_data(err, &rows, &cols);
  if (*err == NO_ERROR)
  {
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = data;
    matrix->name = *matrix_name;
  }
  else
  {
    free(matrix);
    matrix = NULL;
  }
  return matrix;
}

Matrix *add(Matrix *A, Matrix *B)
{
  // adds two matrixes together and returns new Matrix struct
  // Check dimensions
  if (A->rows != B->rows || A->cols != B->cols)
    return NULL;
  Matrix *result = (Matrix *)malloc(sizeof(Matrix));
  if (result == NULL)
  {
    fprintf(stderr, "Malloc error\n");
    exit(MEMMORY_ERROR);
  }
  result->rows = A->rows;
  result->cols = A->cols;
  result->data = create_2D_arr(result->rows, result->cols);
  result->name = NEW_MATRIX;

  for (int row = 0; row < result->rows; ++row)
  {
    for (int col = 0; col < result->cols; ++col)
    {
      result->data[row][col] = (A->data[row][col] + B->data[row][col]);
    }
  }
  return result;
}

Matrix *substract(Matrix *A, Matrix *B)
{
  // substracts two matrixes together and returns new Matrix struct
  // Check dimensions
  if (A->rows != B->rows || A->cols != B->cols)
    return NULL;

  Matrix *result = (Matrix *)malloc(sizeof(Matrix));
  if (result == NULL)
  {
    fprintf(stderr, "Malloc error\n");
    exit(MEMMORY_ERROR);
  }
  result->rows = A->rows;
  result->cols = A->cols;
  result->data = create_2D_arr(result->rows, result->cols);
  result->name = NEW_MATRIX;

  for (int row = 0; row < result->rows; ++row)
  {
    for (int col = 0; col < result->cols; ++col)
    {
      result->data[row][col] = (A->data[row][col] - B->data[row][col]);
    }
  }
  return result;
}

Matrix *multiply(Matrix *A, Matrix *B)
{
  // multiply two matrixes together and returns new Matrix struct
  // Check dimensions
  if (A->cols != B->rows)
    return NULL;
  Matrix *result = (Matrix *)malloc(sizeof(Matrix));
  if (result == NULL)
  {
    fprintf(stderr, "Malloc error\n");
    exit(MEMMORY_ERROR);
  }
  result->rows = A->rows;
  result->cols = B->cols;
  result->data = create_2D_arr(result->rows, result->cols);
  result->name = NEW_MATRIX;

  for (int col = 0; col < B->cols; ++col)
  {
    for (int row_a = 0; row_a < A->rows; ++row_a)
    {
      int sum = 0;
      for (int row_b = 0; row_b < B->rows; ++row_b)
      {
        sum += (A->data[row_a][row_b] * B->data[row_b][col]);
      }
      result->data[row_a][col] = sum;
    }
  }
  return result;
}

int **load_matrix_data(int *err, int *rows, int *cols)
{
  // loads all matrix data
  // checks if all rows have same number of numbers
  // formats the data in 2D int array

  int nums_in_row = -1;
  int current_nums_in_row = 0;
  int num_rows = 0;
  char input_char = getchar();

  int row_size = INIT_SIZE;
  int data_size = INIT_SIZE;
  int **data = (int **)malloc(data_size * sizeof(int *));
  int *row = (int *)malloc(row_size * sizeof(int));
  if (data == NULL || row == NULL)
  {
    fprintf(stderr, "Malloc Error\n");
    exit(MEMMORY_ERROR);
  }
  data[0] = row;

  while (input_char != '\n')
  {
    if ((input_char >= '0' && input_char <= '9') || input_char == '-')
    {
      ungetc(input_char, stdin);
      fflush(stdin);
      int number;
      if (scanf("%d", &number) != 1)
      {
        *err = INPUT_ERROR;
        break;
      }
      if (num_rows != 0 && current_nums_in_row == nums_in_row)
      {
        *err = INPUT_ERROR;
        break;
      }
      data[num_rows][current_nums_in_row++] = number;
      if (num_rows == 0) // Used for making the first row bigger -> other rows have fixed size
        check_first_row(&row, current_nums_in_row, &row_size);
    }
    else if (input_char == ';')
    {
      if (current_nums_in_row < 1)
      {
        *err = INPUT_ERROR;
        break;
      }
      if (num_rows == 0)
        nums_in_row = current_nums_in_row;
      else if (current_nums_in_row < nums_in_row)
      {
        *err = INPUT_ERROR;
        break;
      }
      num_rows++;
      int *new_row = (int *)malloc(nums_in_row * sizeof(int));
      if (new_row == NULL)
      {
        fprintf(stderr, "Memmory Error\n");
        exit(MEMMORY_ERROR);
      }
      data[num_rows] = new_row;
      current_nums_in_row = 0;
      realloc_2D_array(&data, num_rows, &data_size);
    }
    else if (input_char == ']')
    {
      if (num_rows == 0)
        nums_in_row = current_nums_in_row;
      else if (current_nums_in_row != nums_in_row)
      {
        *err = INPUT_ERROR;
        break;
      }
    }

    input_char = getchar();
  }
  if (*err == INPUT_ERROR)
  {
    clean_up(data, num_rows);
    data = NULL;
  }
  *rows = num_rows + 1;
  *cols = nums_in_row;
  return data;
}

void clean_up(int **data, int num_rows)
{
  // cleans up dynamically allocated 2D int array
  for (int i = 0; i <= num_rows; ++i)
  {
    free(data[i]);
    data[i] = NULL;
  }
  free(data);
}

void check_first_row(int **row, int nums_in_row, int *row_size)
{
  // checks wheter row reached its limit size if yes
  // reallocs to size + INIT_SIZE
  if (nums_in_row == *row_size)
  {
    *row_size += INIT_SIZE;
    int *tmp = realloc(*row, *row_size * sizeof(int));
    if (tmp != NULL)
    {
      *row = tmp;
      tmp = NULL;
    }
    else
    {
      fprintf(stderr, "Realloc error !\n");
      exit(MEMMORY_ERROR);
    }
  }
}

void realloc_2D_array(int ***data, int num_rows, int *data_size)
{
  // reallocs 2D int array to size + INIT_SIZE
  if (((*data_size) - 1) == num_rows)
  {
    *data_size += INIT_SIZE;
    int **tmp = realloc(*data, *data_size * sizeof(int *));
    if (tmp != NULL)
    {
      *data = tmp;
      tmp = NULL;
    }
    else
    {
      fprintf(stderr, "Ralloc error \n");
      exit(MEMMORY_ERROR);
    }
  }
}

int **create_2D_arr(int rows, int cols)
{
  // creates 2D int array with given row and cols
  int **data = (int **)malloc(rows * sizeof(int *));
  if (data == NULL)
  {
    fprintf(stderr, "Malloc error\n");
    exit(MEMMORY_ERROR);
  }
  for (int r = 0; r < rows; ++r)
  {
    int *row = (int *)calloc(cols, sizeof(int));
    if (row == NULL)
    {
      fprintf(stderr, "Malloc error\n");
      exit(MEMMORY_ERROR);
    }
    data[r] = row;
  }
  return data;
}
