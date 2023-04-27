#include <stdio.h>

int main(int argc, char *argv[]) 
{
   int a[] = {1,2,3};
   int *p = a;

   int size = sizeof(a) / sizeof(int);
   for (int i = 0; i < size; ++i) {
      printf("a[%d]=%d\n", i, a[i]);
   }

   for (int i = 0; i < size; ++i, p += 1) {
      printf("address %p array adress %p\n", p, &a[i]);
   }

   printf("\nString example\n");
   char str[] = "hallo";
   size = sizeof(str); // consider the null terminating string
   char* strp = str; 
   for (int i = 0; i < size; ++i, strp++) {
      printf("str[%i]=%c address %p\n", i, *strp, strp);
   }
   return 0;
}
