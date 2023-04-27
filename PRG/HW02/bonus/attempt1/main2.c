#include <stdio.h>
#include <stdlib.h>

#define MAX_DIGITS 100
#define INPUT_ERROR 101
#define END_OF_INPUT 102
#define MAX_POWER 7
#define PRIMES_IN_MILLION 78498
#define UPPER_LIMIT 1000000
#define CANT_BE_DIVIDED -1
#define NO_REMAINDER 0
#define TRUE 1
#define FALSE 0
#define OK 0

const int pow_of_ten[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};

int load_num(short number[]);
int make_int(short number[], int size);
int count_digits(int num);
void decompose(short divident[], int divident_len, int primes[]);
void make_array(short output[], int length, int num);
int divide(short divident[], int divident_len, int divisor);
void calc_primes(int primes_list[]);
int count_zeros(short number[]);

int main(void)
{
    short number[MAX_DIGITS] = {0};
    int primes[PRIMES_IN_MILLION];
    int num_digits, err;
    calc_primes(primes);
    for (;;)
    {
        err = load_num(number);

        if (err != INPUT_ERROR && err != END_OF_INPUT)
        {
            num_digits = err - 1;
            printf("input num (%d): ", num_digits);
            for (int i = 0; i < num_digits; ++i)
            {
                printf("%d", number[i]);
            }
            printf("\n");
            short divident[num_digits];

            for (int i = 0; i < num_digits; ++i)
            {
                divident[i] = number[i];
            }

            for (int j = 0; j < 1; ++j)
            {
                printf("error: %d\n", divide(divident, num_digits, 995663));
                for (int i = 0; i < num_digits; ++i)
                {
                    printf("%d", divident[i]);
                }
                printf("\n");
            }
             for (int j = 0; j < 0; ++j)
            {
                printf("error: %d\n", divide(divident, num_digits, 995669));
                for (int i = 0; i < num_digits; ++i)
                {
                    printf("%d", divident[i]);
                }
                printf("\n");
            }

            // decompose(divident, num_digits, primes);
        }
        else if (err == END_OF_INPUT)
        {
            printf("End of input\n");
            err = OK;
            break;
        }
        else
        {
            fprintf(stderr, "Error: Chybny vstup!\n");
            break;
        }
    }
    return err;
}

void decompose(short divident[], int divident_len, int primes[])
{

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

int count_zeros(short number[])
{
    int i = 0;
    while (number[i] == 0)
    {
        ++i;
    }
    return i;
}

int divide(short divident_in[], int divident_len_in, int divisor)
{
    // overwrites divident with (divident / divisor) is remainder is 0
    // return value 0 if it can be divided without remainder
    //  if not returns -1

    // copy into new array that will be changed
    short divident[divident_len_in];
    for (int i = 0; i < divident_len_in; ++i)
    {
        divident[i] = divident_in[i];
    }
    // bunch of variables needed in computation (I think at least)
    int divident_len = divident_len_in;
    int divisor_digits = count_digits(divisor);
    int tmp_divident_length = divisor_digits + 1;
    int division_index = 0;
    int output_len = 0;
    short output[MAX_DIGITS] = {0};
    int last_division = FALSE;

    // do some random calculation and hope it works out
    // currently does not work for dividing by 10^x - dont know how to fix it
    // so i wont fix it for now (i dont need it to work 10^x is not a prime so i wont be dividing by it)
    for (;;)
    {
        if (tmp_divident_length > divident_len)
        {
            tmp_divident_length = divident_len;
            last_division = TRUE;
        }
        short tmp_divident[tmp_divident_length];
        for (int i = 0; i < tmp_divident_length; ++i)
        {
            tmp_divident[i] = divident[division_index + i];
        }
        int divident_int = make_int(tmp_divident, tmp_divident_length);
        int div = divident_int / divisor;
        int remainder = divident_int - (div * divisor);
        if (remainder != 0 || divident_len != tmp_divident_length)
        {
            div *= pow_of_ten[divisor_digits - count_digits(remainder + 1)];
        }
        printf("divident: %d div: %d | remainder: %d | divident len: %d | %d\n", divident_int, div, remainder, divident_len, last_division);
        // save the results
        if (last_division == TRUE && remainder != 0)
        {
            // printf("cant be divided \n");
            return CANT_BE_DIVIDED;
        }
        if (last_division == TRUE && remainder == 0 && div == 0)
        {
            // overwrite devider -> this function is only used for prime decomposition
            // so it is needed anyways
            printf("calculated num: ");
            for (int i = 0; i < output_len; ++i)
            {
                printf("%d ", output[i]);
            }
            printf("\n");
            int len_dt = divident_len_in - output_len;
            for (int i = 0; i < divident_len_in; ++i)
            {
                if (i >= len_dt)
                {
                    divident_in[i] = output[i - len_dt];
                }
                else
                {
                    divident_in[i] = 0;
                }
            }

            return NO_REMAINDER;
        }
        int remainder_len = count_digits(remainder);
        short remainder_array[remainder_len];
        make_array(remainder_array, remainder_len, remainder);
        int index_dt = tmp_divident_length - remainder_len;
        division_index += index_dt;
        divident_len -= index_dt;
        for (int i = 0; i < remainder_len; ++i)
        {
            divident[division_index + i] = remainder_array[i];
        }
        int div_len = count_digits(div);
        short div_array[div_len];
        make_array(div_array, div_len, div);
        for (int i = 0; i < div_len; ++i)
        {
            output[output_len + i] = div_array[i];
            // printf("printing with div len: %d num: %d \n",div_len, output[output_len + i]);
        }
        output_len += div_len;
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
    /*
    printf("making array from num %d length %d: ", num, length);
    int tmp_l = length;*/
    while (num >= 10)
    {
        output[length - 1] = num % 10;
        --length;
        num /= 10;
    }
    output[0] = num;
    /*for(int i = 0; i < tmp_l; ++i){
        printf("%d ", output[i]);
    }
    printf("\n");*/
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

int load_num(short number[])
{
    char digit_c;
    short digit;
    int array_index = 0;
    int err = OK;
    int flag = TRUE;

    do
    {
        digit_c = getchar();
        digit = (short)atoi(&digit_c);
        if (flag && digit == 0)
        {
            err = END_OF_INPUT;
            break;
        }
        else if (0 > digit || 9 < digit)
        {
            printf("not in range 0 - 9");
            err = INPUT_ERROR;
            break;
        }
        else
        {
            number[array_index++] = digit;
        }
        flag = FALSE;
    } while (digit_c != '\n');
    if (err == OK)
    {
        err = array_index;
    }
    return err;
}