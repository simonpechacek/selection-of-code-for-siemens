#include <stdio.h>

int main(void)
{
   int a[] = {1, 2, 3, 4};
   int b[] = {[3] = 10, [1] = 1, [2] = 5, [0] = 0};

   // b = a; It is not possible to assign arrays
   for (int i = 0; i < 4; ++i) {
      printf("a[%i] =%3i   b[%i] =%3i\n", i, a[i], i, b[i]);
   }

   int *p = a; 
   a[2] = 99;

   printf("\nPrint content of the array 'a' using pointer arithmetic\n");
   for (int i = 0; i < 4; ++i) {
      printf("a[%i] =%3i   p+%i =%3i\n", i, a[i], i, *(p+i));
   }

   return 0;
}
