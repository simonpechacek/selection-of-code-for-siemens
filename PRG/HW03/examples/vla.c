#include <stdio.h>

int main(void)
{
   int i, n;
   printf("Enter number of integers to be read: ");
   if (scanf("%d", &n) != 1 && n > 0) {
      return 100;
   }

   int a[n]; /* variable length array */
   for (i = 0; i < n; ++i) {
      if (scanf("%d", &a[i]) != 1) {
         return 101;
      }
   }
   printf("Entered numbers in reverse order: ");
   for (i = n - 1; i >= 0; --i) {
      printf(" %d", a[i]);
   }
   printf("\n");
   return 0;
}
