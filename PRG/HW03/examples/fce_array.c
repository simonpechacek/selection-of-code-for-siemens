#include <stdio.h>

void fce(int array[]);

int main(void)
{
   int array[] = {1, 3, 3};
   fce(array);

   return 0;
}

void fce(int array[])
{
   int local_array[] = {2, 4, 6};
   printf("sizeof(array) = %lu -- sizeof(local_array) = %lu\n", 
         sizeof(array), sizeof(local_array));
   for (int i = 0; i < 3; ++i) {
      printf("array[%i]=%i local_array[%i]=%i\n", i, array[i], i, local_array[i]);
   }
}
