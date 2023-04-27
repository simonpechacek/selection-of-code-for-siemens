/***************************************************************
function: prime decomposition

=> pre-calculate primes to 1_000_000 using Eratosthens sieve

author: Šimon Pecháček

***************************************************************/

#include <stdio.h>
#include <stdlib.h>

// CONSTANTS //
#define UPPER_LIMIT 1000000
#define PRIMES_IN_MILLION 78498
#define TRUE 1
#define FALSE 0
#define OK 0
#define INPUT_ERROR 100
#define END_OF_INPUT 101
#define IN_CNT 1

// FUNCTIONS //
void calc_primes(int primes_list[]);
int decompose(long int num, int primes[]);
int load_input(long int *input_num);

int main(int argc, char *argv[])
{
  int primes[PRIMES_IN_MILLION];
  long int input_num;
  int err;

  calc_primes(primes);

  for (;;)
  {
    err = load_input(&input_num);
    if (err == INPUT_ERROR)
    {
      break;
    }
    else if (err == END_OF_INPUT)
    {
      err = OK;
      break;
    }
    else
    {
      decompose(input_num, primes);
    }
  }
  return err;
}

int load_input(long int *input_num)
{ 
  // Loads input and checks if its valid
  int err = OK;

  if (scanf("%ld", input_num) == IN_CNT)
  {
    if (*input_num < 0)
    {
      err = INPUT_ERROR;
      fprintf(stderr, "Error: Chybny vstup!\n");
    }
    else if (*input_num == 0)
    {
      err = END_OF_INPUT;
    }
  }
  else
  {
    err = INPUT_ERROR;
    fprintf(stderr, "Error: Chybny vstup!\n");
  }
  return err;
}

int decompose(long int num, int primes[])
{
  // Decompose input number into primes
  printf("Prvociselny rozklad cisla %ld je:\n", num);
  // special case of num being "1"
  if (num == 1)
  {
    printf("1\n");
  }
  else
  {
    for (int i = 0; i < PRIMES_IN_MILLION; ++i)
    {

      int exponent = 0;
      // Devide by this prime till you can
      while (num % primes[i] == 0)
      {
        num /= primes[i];
        ++exponent;
      }

      // If num could be devided at least once print it
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
        if (num > primes[i])
        {
          printf(" x ");
        }
      }

      // can't be devided more
      if (num < primes[i])
      {
        printf("\n");
        break;
      }
    }
  }
  return OK;
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
