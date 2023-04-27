#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define INIT_SIZE 10
#define LETTERS_IN_ALPHABET 26

#define TRUE 1
#define FALSE 0

#define OK 0
#define INPUT_ERROR 100
#define ERROR_OFFSET 100

const char letter_shift[] = {'A', 'a'};
const char *error_messages[] = {"Error: Chybny vstup!\n", "Error: Chybna delka vstupu!\n"};


//Used functions
char *load_line(int *str_len, int *err);
char shift_char(char in, int offset);
void shift(const char *in, int in_len, char *out, int offset);
void print_cipher(const char *cipher, int cipher_len, int offset);
void decipher(const char *cipher, int cipher_len, const char *message, int message_len);
void mem_release(void **pointer);
int levenshtein_distance(const char *s1, int len_s1, const char *s2, int len_s2);
int **init_array(int len_s1, int len_s2);
void print_array(int **array, int rows, int cols);
int min3(int a, int b, int c);
void free_array(int **array, int rows);

int main(void)
{

    int cipher_len = 0, message_len = 0, err = OK;
    char *cipher = NULL;
    char *message = NULL;

    cipher = load_line(&cipher_len, &err);
    if (!err)
    {
        message = load_line(&message_len, &err);
    }

    if (err)
        fprintf(stderr, "%s", error_messages[err - ERROR_OFFSET]);
    else
    {
        decipher(cipher, cipher_len, message, message_len);
        //levenshtein_distance(cipher, cipher_len, message, message_len);
    }
    // free allocated memory
    mem_release((void **)&cipher);
    mem_release((void **)&message);
    return err;
}

void decipher(const char *cipher, int cipher_len, const char *message, int message_len)
{
    // Try to shift the message from 0 - 52, after that it start over so there is no point
    // Compare input message and shifted message
    // Get the best match shift
    char *shifted_cipher = (char *)malloc(cipher_len * sizeof(char));
    if (shifted_cipher == NULL)
    {
        fprintf(stderr, "Memmory allocation fail");
        exit(-1);
    }
    int best_match = cipher_len + message_len, best_offset, distance;
    for (int i = 0; i < LETTERS_IN_ALPHABET * 2; ++i)
    {
        shift(cipher, cipher_len, shifted_cipher, i);
        distance = levenshtein_distance(shifted_cipher, cipher_len, message, message_len);
        if (distance < best_match)
        {
            best_match = distance;
            best_offset = i;
        }
    }
    mem_release((void **)&shifted_cipher);
    print_cipher(cipher, cipher_len, best_offset);
}

void mem_release(void **ptr)
{
    // This is used for freing dynamically allocated memory
    if (ptr != NULL && *ptr != NULL)
    {
        free(*ptr);
        *ptr = NULL;
    }
}

void print_cipher(const char *cipher, int cipher_len, int offset)
{
    // prints out input cipher with best offset
    char *output = malloc((cipher_len + 1) * sizeof(char));
    if (output == NULL)
    {
        fprintf(stderr, "Memmory allocation fail");
        exit(-1);
    }
    output[cipher_len] = '\0';
    shift(cipher, cipher_len, output, offset);
    printf("%s\n", output);
    mem_release((void **)&output);
}


int levenshtein_distance(const char *s1, int len_s1, const char *s2, int len_s2)
{
    // array of [row][cols] where row = len_s1 + 1, col = len_s2 + 1
    // for some reason strings are 1 indexed in all algorithms
    // so i will have to kinda one index them too

    int **array = init_array(len_s1, len_s2);
    int distance;
    for (int i = 1; i < len_s1 + 1; ++i)
    {
        for (int j = 1; j < len_s2 + 1; ++j)
        {
            int cost;
            if (s1[i - 1] == s2[j - 1])
                cost = 0;
            else
                cost = 1;

            array[i][j] = min3(array[i - 1][j] + 1, array[i][j - 1] + 1, array[i - 1][j - 1] + cost);
        }
    }
    //print_array(array, len_s1 + 1, len_s2 + 1);
    distance = array[len_s1][len_s2];
    free_array(array, len_s1 + 1);  
    //printf("distance: %d\n", distance);
    return distance;
}

void print_array(int **array, int rows, int cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            printf("%2d ", array[i][j]);
        }
        putchar('\n');
    }
}

void free_array(int **array, int rows)
{
    for (int i = 0; i < rows; ++i)
    {
        free(array[i]);
        array[i] = NULL;
    }
    free(array);
    array = NULL;
}

int min3(int a, int b, int c)
{
    // returns minimum out of 3 values
    if (a < b)
    {
        if (a < c)
        {
            return a;
        }
        else
        {
            return c;
        }
    }
    else
    {
        if (b < c)
        {
            return b;
        }
        else
        {
            return c;
        }
    }
}

int **init_array(int len_s1, int len_s2)
{
    // make the array
    int **array = (int **)malloc((len_s1 + 1) * sizeof(int *));
    if (array == NULL)
    {
        fprintf(stderr, "Memmory allocation fail");
        exit(-1);
    }
    for (int i = 0; i < len_s1 + 1; ++i)
    {
        array[i] = (int *)malloc((len_s2 + 1) * sizeof(int));
        if (array[i] == NULL)
        {
            fprintf(stderr, "Memmory allocation fail");
            exit(-1);
        }
    }
    // initialize first row and col
    for (int i = 0; i < len_s1 + 1; ++i)
    {
        array[i][0] = i;
    }
    for (int i = 0; i < len_s2 + 1; ++i)
    {
        array[0][i] = i;
    }
    return array;
}

void shift(const char *in, int in_len, char *out, int offset)
{
    // shift in string by offset
    for (int i = 0; i < in_len; ++i)
    {
        out[i] = shift_char(in[i], offset);
    }
}

char shift_char(char in, int offset)
{
    // shifts one char by offset
    int lower, overflow;
    if (in >= 'a')
        lower = TRUE;
    else
        lower = FALSE;

    in -= letter_shift[lower];
    in += offset;
    overflow = in / LETTERS_IN_ALPHABET;
    in %= LETTERS_IN_ALPHABET;
    in += letter_shift[(overflow + lower + 1) % 2];
    return in;
}

char *load_line(int *str_len, int *err)
{
    // loads one line from input stream
    int size = INIT_SIZE;
    char in;
    char *str = (char *)malloc(size * sizeof(char));

    if (str == NULL)
    {
        fprintf(stderr, "Error: required block of memory has not been allocated!\n");
        exit(-1);
    }

    in = getchar();

    while (in != '\n')
    {
        if (isalpha(in))
        {
            // printf("is alpha, str_len: %d\n", *str_len);
            str[(*str_len)++] = in;
            if (*str_len == size - 1)
            {
                // double the lenght of array
                size *= 2;
                char *tmp = realloc(str, size * sizeof(char));
                if (tmp)
                {
                    str = tmp;
                }
                else
                {
                    fprintf(stderr, "Error: required block of memory has not been allocated!\n");
                    exit(-1);
                }
            }
        }
        else
        {
            *err = INPUT_ERROR;
            break;
        }
        in = getchar();
    }
    if ((*err) == OK)
    {
        str[*str_len] = '\0';
    }
    else
    {
        free(str);
        str = NULL;
    }
    return str;
}
