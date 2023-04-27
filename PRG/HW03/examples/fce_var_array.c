#include <stdio.h>

void fce(int n);

int main(int argc, char *argv[])
{
   fce(argc);
   return 0;
}

void fce(int n)
{
   int local_array[n]; // variable length array

   printf("sizeof(local_array) = %lu\n", sizeof(local_array));
   printf("length of array = %lu\n", sizeof(local_array) / sizeof(int));
   for (int i = 0; i < n; ++i) {
      local_array[i] = i * i;
   }
}
