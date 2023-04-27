#include <stdio.h>

static const double pi = 3.14;

double do_nothing(int v);
double pi_multiply(int v);
double pi_pow(int v);

int main(int argc, char *argv[])
{
   double (*function_p)(int v);
   if (argc > 1) {
      char *argv1 = argv[1];
      switch (*argv1) {
         case 'm':
            function_p = pi_multiply;
            break;
         case 'p':
            function_p = pi_pow;
            break;
         default:
            function_p = do_nothing;
            break;
      }
   } else {
      function_p = do_nothing; /* NOTE: repeated to show default in in the switch */
   }
   double r = (*function_p)(2);
   printf("Return value of the function %lf\n", r);
   return 0;
}

double pi_multiply(int v)
{
   return pi * v;
}

double pi_pow(int v)
{
   double r = 3.14;
   for (int i = 0; i < v; ++i) {
      r *= pi;
   }
   return r;
}

double do_nothing(int v)
{
   return pi;
}
