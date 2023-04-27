#include <stdio.h>
#include <stdlib.h>

#define MAX_POWER 7
#define MAX_DIGITS 100
#define CANT_BE_DIVIDED -1
#define NO_REMAINDER 0
#define PRIMES_IN_MILLION 78498
#define UPPER_LIMIT 1000000
#define TRUE 1
#define FALSE 0
#define OK 0
#define INPUT_ERROR 101
#define END_OF_INPUT 102

const int pow_of_ten[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};

int make_int(short number[], int size);
int count_digits(int num);
void make_array(short output[], int length, int num);
int divide(short divident[], int div_len, int divisor);
void save_output(short divident_in[], short result[], int result_index, int div_len);
void save_results(int remainder, int *start_index, int i_dt, short divident[]);
int load_num(short number[]);
void decompose(short divident[], int divident_len, int primes[]);
void calc_primes(int primes_list[]);
int count_zeros(short number[]);
void print_num(short number[], int num_len);

int main(void)
{
    int num_digits, err;
    short number[MAX_DIGITS] = {0};
    int primes[PRIMES_IN_MILLION];

    calc_primes(primes);

    for (;;)
    {
        err = load_num(number);
        if (err != INPUT_ERROR && err != END_OF_INPUT)
        {
            num_digits = err;
            short divident[num_digits];
            for (int i = 0; i < num_digits; ++i)
            {
                divident[i] = number[i];
            }
            // printf("Decomposing: \n");
            decompose(divident, num_digits, primes);
        }
        else if (err == END_OF_INPUT)
        {
            err = OK;
            break;
        }
        else
        {
            err--;
            fprintf(stderr, "Error: Chybny vstup!\n");
            break;
        }
    }
    return err;
}

void decompose(short divident[], int divident_len, int primes[])
{
    printf("Prvociselny rozklad cisla ");
    print_num(divident, divident_len);
    printf(" je:\n");
    if (divident_len == 1 && divident[0] == 1)
    {
        printf("1\n");
        return;
    }
    for (int i = 0; i < PRIMES_IN_MILLION; ++i)
    {
        int divisor = primes[i];
        int exponent = 0;
        int result;

        result = divide(divident, divident_len, divisor);
        while (result == NO_REMAINDER)
        {

            result = divide(divident, divident_len, divisor);
            ++exponent;
        }

        if (exponent)
        {
            // if exponent is 1 dont print exponent if its bigger print it
            if (exponent > 1)
            {
                printf("%d^%d", primes[i], exponent);
            }
            else
            {
                printf("%d", primes[i]);
            }
            // check if there will be more number to print if yes print separator
            int len_dt = divident_len - count_zeros(divident);
            if (len_dt >= count_digits(primes[i]))
            {
                if (len_dt == count_digits(primes[i]))
                {
                    short tmp_num[len_dt];
                    for (int j = 0; j < len_dt; j++)
                    {
                        tmp_num[j] = divident[divident_len - len_dt + j];
                    }
                    if (make_int(tmp_num, len_dt) > primes[i])
                    {
                        printf(" x ");
                    }
                }
                else
                {
                    printf(" x ");
                }
            }
        }

        // can't be devided more
        if (divident_len - count_zeros(divident) < count_digits(primes[i]))
        {
            printf("\n");
            break;
        }
    }
}

void print_num(short number[], int num_len)
{
    for (int i = 0; i < num_len; ++i)
    {
        printf("%d", number[i]);
    }
}

void calc_primes(int primes_list[])
{
    // calculates primes < 1_000_000
    short eratos[UPPER_LIMIT];
    // init array with all 1
    for (int i = 0; i < UPPER_LIMIT; ++i)
    {
        eratos[i] = TRUE;
    }

    int prime_index = 0;

    // Eratosthenes - start at number 2 end on 999_999 (1 milion is not a prime)
    for (int i = 2; i < UPPER_LIMIT; ++i)
    {
        // if not prime - continue
        if (!eratos[i])
        {
            continue;
        }
        else
        {
            // we found prime
            primes_list[prime_index] = i;
            ++prime_index;
            // remove all multiples
            int mult = i;
            int cnt = 1;
            while (mult < UPPER_LIMIT)
            {
                eratos[mult] = FALSE;
                mult = (++cnt) * i;
            }
        }
    }
}

int divide(short divident_in[], int div_len, int divisor)
{
    // make a copy of input number - to not overwite it if it cant be divided
    short divident[div_len];
    for (int i = 0; i < div_len; ++i)
    {
        divident[i] = divident_in[i];
    }
    int divisor_digits = count_digits(divisor);
    if (divisor_digits > div_len)
    {
        return CANT_BE_DIVIDED;
    }
    // end_index - including number on end_index
    int start_index = 0, end_index = 0, result_index = 0;
    int flag = TRUE;
    short result[MAX_DIGITS] = {0};

    // end_index = set_end_index(divisor_digits, div_len);
    while (end_index < div_len)
    {
        int i_dt = end_index - start_index + 1;
        short tmp_div[i_dt];
        for (int i = start_index; i <= end_index; ++i)
        {
            tmp_div[i - start_index] = divident[i];
        }
        int div_int, div, remainder;
        div_int = make_int(tmp_div, i_dt);
        div = div_int / divisor;

        if (div == 0 && flag)
        {
            ++end_index;
            continue;
        }

        remainder = div_int - (div * divisor);
        // save the results
        save_results(remainder, &start_index, i_dt, divident);
        result[result_index++] = div;
        ++end_index;
        flag = FALSE;
        if (end_index == div_len)
        {
            // division done
            if (remainder == 0)
            {
                save_output(divident_in, result, result_index, div_len);
                return NO_REMAINDER;
            }
            else
            {
                return CANT_BE_DIVIDED;
            }
        }
    }
    return CANT_BE_DIVIDED;
}
void save_results(int remainder, int *start_index, int i_dt, short divident[])
{
    int remainder_len = count_digits(remainder);
    short rem_arr[remainder_len];
    make_array(rem_arr, remainder_len, remainder);
    *start_index += (i_dt - remainder_len);
    for (int i = *start_index; i < *start_index + remainder_len; ++i)
    {
        divident[i] = rem_arr[i - *start_index];
    }
}

void save_output(short divident_in[], short result[], int result_index, int div_len)
{
    int len_dt = div_len - result_index;
    for (int i = 0; i < div_len; ++i)
    {
        if (i >= len_dt)
        {
            divident_in[i] = result[i - len_dt];
        }
        else
        {
            divident_in[i] = 0;
        }
    }
}

int count_digits(int num)
{
    // returns number of digits in number < 10_000_000
    int digits = 1;

    for (int i = 1; i < MAX_POWER + 1; ++i)
    {
        if (num >= pow_of_ten[i])
        {
            continue;
        }
        else
        {
            digits = i;
            break;
        }
    }
    return digits;
}

void make_array(short output[], int length, int num)
{
    while (num >= 10)
    {
        output[length - 1] = num % 10;
        --length;
        num /= 10;
    }
    output[0] = num;
}

int make_int(short number[], int size)
{
    // max size is 7 - biggest prime devisor is < 1_000_000 so max number passed here is 1 digit longer
    // if length is 7 - this means the number will have 7 digits
    int result = 0;
    for (int i = 0; i < size; ++i)
    {
        result += pow_of_ten[(size - i - 1)] * number[i];
        // printf("step %d, result: %d\n", i, result);
    }
    return result;
}

int count_zeros(short number[])
{
    int i = 0;
    while (number[i] == 0)
    {
        ++i;
    }
    return i;
}

int load_num(short number[])
{
    char digit;
    int array_index = 0, err = OK, flag = TRUE;
    digit = getchar();

    while (digit != '\n')
    {
        if (digit == '0' && flag)
        {
            err = END_OF_INPUT;
            break;
        }
        else if ('0' > digit || '9' < digit)
        {
            err = INPUT_ERROR;
            break;
        }
        else
        {
            number[array_index++] = (short)atoi(&digit);
        }
        flag = FALSE;
        digit = getchar();
    }

    if (err == OK)
    {
        err = array_index;
    }
    return err;
}
