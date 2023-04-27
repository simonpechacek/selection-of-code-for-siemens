#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define INIT_SIZE 10
#define LETTERS_IN_ALPHABET 26

#define TRUE 1
#define FALSE 0

#define OK 0
#define INPUT_ERROR 100
#define NOT_SAME_LENGTH_ERROR 101
#define ERROR_OFFSET 100

const char letter_shift[] = {'A', 'a'};
const char *error_messages[] = {"Error: Chybny vstup!\n", "Error: Chybna delka vstupu!\n"};

char *load_line(int *str_len, int *err);
char shift_char(char in, int offset);
void shift(const char *in, int in_len, char *out, int offset);
void print_cipher(const char *cipher, int cipher_len, int offset);
void decipher(const char *cipher, int cipher_len, const char *message);
int compare(const char *in, const char *message, int len);
void mem_release(void **pointer);

int main(void)
{
    int cipher_len = 0, message_len = 0, err = OK;
    char *cipher = NULL;
    char *message = NULL;

    cipher = load_line(&cipher_len, &err);
    if (!err)
    {
        message = load_line(&message_len, &err);
        if (!err)
        {
            if (cipher_len != message_len)
            {
                err = NOT_SAME_LENGTH_ERROR;
            }
        }
    }

    if (err)
        fprintf(stderr, "%s", error_messages[err - ERROR_OFFSET]);
    else
        decipher(cipher, cipher_len, message);

    // free allocated memory
    mem_release((void **)&cipher);
    mem_release((void **)&message);
    return err;
}

void decipher(const char *cipher, int cipher_len, const char *message)
{
    // cipher and message len should be the same but not in the bonus HW
    // Try to shift the message from 0 - 52, after that it start over so there is no point
    // Compare input message and shifted message
    // Get the best match shift
    char *shifted_cipher = malloc(cipher_len * sizeof(char));
    if (shifted_cipher == NULL)
    {
        fprintf(stderr, "Memmory allocation fail");
        exit(-1);
    }
    int best_match = -1, best_offset, num_matched_letters;
    for (int i = 0; i < LETTERS_IN_ALPHABET * 2; ++i)
    {
        shift(cipher, cipher_len, shifted_cipher, i);
        num_matched_letters = compare(shifted_cipher, message, cipher_len);
        if (num_matched_letters > best_match)
        {
            best_match = num_matched_letters;
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
    mem_release((void**)&output);
}

int compare(const char *in, const char *message, int len)
{
    // compares number of same letters in 2 strings
    int same_letters = 0;
    for (int i = 0; i < len; ++i)
    {
        if (in[i] == message[i])
        {
            same_letters++;
        }
    }
    return same_letters;
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
