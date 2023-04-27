#include <stdio.h>
#include <stdlib.h>

#define INPUT_ERROR 100
#define NO_ERROR 0
#define MEMMORY_ERR -1
#define NUM_OPERATIONS 3
#define WRONG_OPERATION 404
#define ADD 0
#define SUBSTRACT 1
#define MULTIPLY 2
#define TWO_MAT 2
#define THREE_MAT 3
#define BUFFER_SIZE 3

struct Matrix
{
    int rows;
    int cols;
    int **data;
};

typedef struct Matrix Matrix;

const char operations[] = {'+', '-', '*'};

// LOADING DATA TO 2D ARRAY
int **load_matrix_data(int rows, int cols);
int **create_2D_arr(int rows, int cols);
// LOADING AND DELETING MATRIXES
Matrix *load_matrix();
void free_matrix(Matrix *matrix);
void print_matrix(Matrix *matrix);
void free_matrix_buffer(Matrix **matrix_buffer);
// MATRIX FUNCTIONS
Matrix *add(Matrix *A, Matrix *B);
Matrix *substract(Matrix *A, Matrix *B);
Matrix *multiply(Matrix *A, Matrix *B);
// USED FOR SOLVING
int solve();
int first_load(Matrix **matrix_buffer, int *operations);
void left_shift(Matrix **matrix_buffer, int *operations);
int advanced_solve(Matrix **matrix_buffer, int *operations);
int load_more(Matrix **matrix_buffer, int *operations);
int two_mat_solve(Matrix **matrix_buffer, int *operations);
int operation_type();

Matrix *(*matrix_operations[3])(Matrix *, Matrix *) = {add, substract, multiply};

int main(int argc, char *argv[])
{
    int ret;
    ret = solve();
    if (ret == INPUT_ERROR)
        fprintf(stderr, "Error: Chybny vstup!\n");
    return ret;
}

int solve()
{
    // SOLVES GIVEN MATRIx EQUATION
    Matrix *matrix_buffer[3] = {NULL, NULL, NULL};
    int operations[2] = {-1, -1};
    int ret;
    ret = first_load(matrix_buffer, operations);
    if (ret == INPUT_ERROR)
        return ret;
    else if (ret == TWO_MAT)
    {
        ret = two_mat_solve(matrix_buffer, operations);
    }
    else
    {
        ret = advanced_solve(matrix_buffer, operations);
        if (ret == NO_ERROR)
            ret = two_mat_solve(matrix_buffer, operations);
    }
    return ret;
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

    free_matrix(matrix_buffer[0]);
    free_matrix(matrix_buffer[1]);
    free_matrix(result);
    return ret;
}

int advanced_solve(Matrix **matrix_buffer, int *operations)
{
    // Solves advanced matrix equation - operation order matters !!
    while (1)
    {
        if (operations[0] == MULTIPLY)
        {
            // just multiply 2 matrixes
            Matrix *result = (*matrix_operations[operations[0]])(matrix_buffer[0], matrix_buffer[1]);
            free_matrix(matrix_buffer[0]);
            free_matrix(matrix_buffer[1]);
            if (result == NULL)
            {
                free_matrix(matrix_buffer[2]);
                return INPUT_ERROR;
            }
            matrix_buffer[0] = result;
            left_shift(matrix_buffer, operations);
            int ret = load_more(matrix_buffer, operations);
            if (ret == EOF)
                return NO_ERROR;
            if (ret == INPUT_ERROR)
            {
                free_matrix(matrix_buffer[0]);
                free_matrix(matrix_buffer[1]);
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
                free_matrix(matrix_buffer[0]);
                free_matrix(matrix_buffer[1]);
                if (result == NULL)
                {
                    free_matrix(matrix_buffer[2]);
                    return INPUT_ERROR;
                }
                matrix_buffer[0] = result;
                left_shift(matrix_buffer, operations);
                int ret = load_more(matrix_buffer, operations);
                if (ret == EOF)
                    return NO_ERROR;
                if (ret == INPUT_ERROR)
                {
                    free_matrix(matrix_buffer[0]);
                    free_matrix(matrix_buffer[1]);
                    return INPUT_ERROR;
                }
            }
            else
            {
                // if next operation is multiplication do that
                Matrix *result = (*matrix_operations[operations[1]])(matrix_buffer[1], matrix_buffer[2]);
                free_matrix(matrix_buffer[1]);
                free_matrix(matrix_buffer[2]);
                if (result == NULL)
                {
                    free_matrix(matrix_buffer[0]);
                    return INPUT_ERROR;
                }
                matrix_buffer[1] = result;
                int ret = load_more(matrix_buffer, operations);
                if (ret == EOF)
                    return NO_ERROR;
                if (ret == INPUT_ERROR)
                {
                    free_matrix(matrix_buffer[0]);
                    free_matrix(matrix_buffer[1]);
                    return INPUT_ERROR;
                }
            }
        }
    }
}

int load_more(Matrix **matrix_buffer, int *operations)
{
    // loads next operation and matrix to buffers
    int operation = operation_type();
    if (operation == EOF)
        return EOF;
    if (operation == WRONG_OPERATION)
        return INPUT_ERROR;

    operations[1] = operation;
    Matrix *C = load_matrix();
    if (C == NULL)
        return INPUT_ERROR;
    matrix_buffer[2] = C;
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

int first_load(Matrix **matrix_buffer, int *operations)
{
    // loads first 3 Matricies (its a bit dirty but it should cover all initial problems)
    int operation;
    Matrix *A = load_matrix();
    if (A == NULL)
        return INPUT_ERROR;
    matrix_buffer[0] = A;
    operation = operation_type();
    if (operation == EOF || operation == WRONG_OPERATION)
    {
        free_matrix(matrix_buffer[0]);
        return INPUT_ERROR;
    }
    operations[0] = operation;
    Matrix *B = load_matrix();
    if (B == NULL)
    {
        free_matrix_buffer(matrix_buffer);
        return INPUT_ERROR;
    }
    matrix_buffer[1] = B;
    operation = operation_type();
    if (operation == WRONG_OPERATION)
    {
        free_matrix_buffer(matrix_buffer);
        return INPUT_ERROR;
    }
    if (operation == EOF)
        return TWO_MAT;
    operations[1] = operation;
    Matrix *C = load_matrix();
    if (C == NULL)
    {
        free_matrix_buffer(matrix_buffer);
        return INPUT_ERROR;
    }
    matrix_buffer[2] = C;
    return THREE_MAT;
}
void free_matrix_buffer(Matrix **matrix_buffer)
{
    for (int i = 0; i < BUFFER_SIZE; ++i)
    {
        free_matrix(matrix_buffer[i]);
        matrix_buffer[i] = NULL;
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

void print_matrix(Matrix *matrix)
{
    // prints out matrix
    printf("%d %d\n", matrix->rows, matrix->cols);
    for (int r = 0; r < (matrix->rows); ++r)
    {
        for (int c = 0; c < (matrix->cols); ++c)
        {
            if (c != 0)
                putchar(' ');
            printf("%d", (matrix->data)[r][c]);
        }
        putchar('\n');
    }
}

int operation_type()
{
    // figures out what operation should be used
    char next_op;
    // read the new line char
    getchar();
    next_op = getchar();
    if (next_op == EOF)
        return EOF;
    int operation = WRONG_OPERATION;
    for (int i = 0; i < NUM_OPERATIONS; ++i)
    {
        if (operations[i] == next_op)
        {
            operation = i;
            break;
        }
    }
    return operation;
}

Matrix *add(Matrix *A, Matrix *B)
{
    // adds two matrixes together and returns new Matrix struct
    // Check dimensions
    if (A->rows != B->rows || A->cols != B->cols)
        return NULL;

    Matrix *result = (Matrix *)malloc(sizeof(Matrix));
    result->rows = A->rows;
    result->cols = A->cols;
    result->data = create_2D_arr(result->rows, result->cols);

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
    result->rows = A->rows;
    result->cols = A->cols;
    result->data = create_2D_arr(result->rows, result->cols);

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
    result->rows = A->rows;
    result->cols = B->cols;
    result->data = create_2D_arr(result->rows, result->cols);

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

Matrix *load_matrix()
{
    // loads all matrix data to Matrix struct
    Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));
    if (matrix == NULL)
    {
        fprintf(stderr, "ERROR malloc failed!\n");
        exit(MEMMORY_ERR);
    }
    // read num rows and cols
    int rows, cols, err = NO_ERROR;
    if (scanf("%d %d", &rows, &cols) != 2)
        err = INPUT_ERROR;
    else
    {
        matrix->rows = rows;
        matrix->cols = cols;
        int **data_holder = load_matrix_data(rows, cols);
        if (data_holder != NULL)
        {
            matrix->data = data_holder;
            data_holder = NULL;
        }
        else
            err = INPUT_ERROR;
    }
    if (err == INPUT_ERROR)
    {
        // free matrix struct and return NULL pointer
        free(matrix);
        matrix = NULL;
    }
    return matrix;
}

int **create_2D_arr(int rows, int cols)
{
    int **data = (int **)malloc(rows * sizeof(int *));
    for (int r = 0; r < rows; ++r)
    {
        int *row = (int *)calloc(cols, sizeof(int));
        data[r] = row;
    }
    return data;
}

int **load_matrix_data(int rows, int cols)
{
    int **data = (int **)malloc(rows * sizeof(int *));
    if (data == NULL)
    {
        fprintf(stderr, "ERROR malloc failed!\n");
        exit(MEMMORY_ERR);
    }
    int num, err = NO_ERROR, alloced_rows = 0;

    for (int r = 0; r < rows; ++r)
    {
        int *row = (int *)malloc(cols * sizeof(int));
        if (row == NULL)
        {
            fprintf(stderr, "ERROR malloc failed!\n");
            exit(MEMMORY_ERR);
        }
        for (int c = 0; c < cols; ++c)
        {
            if (scanf("%d", &num) == 1)
                row[c] = num;
            else
            {
                err = INPUT_ERROR;
                break;
            }
        }
        alloced_rows++;
        data[r] = row;
        if (err == INPUT_ERROR)
            break;
    }
    if (err == INPUT_ERROR)
    {
        // free the alloced memory and return NULL pointer
        for (int m_r = 0; m_r < alloced_rows; ++m_r)
        {
            free(data[m_r]);
            data[m_r] = NULL;
        }
        free(data);
        data = NULL;
    }
    return data;
}
