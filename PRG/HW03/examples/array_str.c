#include <stdio.h>

int main(void)
{
   char str[] = "123";
   char s[] = {'5', '6', '7' };

   printf("Size of str %lu\n", sizeof(str));
   printf("Size of  s  %lu\n", sizeof(s));
   printf("str '%s'\n", str);
   printf(" s  '%s'\n", s);

   {
      printf("\nText string as pointer to char\n");
      char* sp = "ABC";
      printf("Size of ps  %lu\n", sizeof(sp));
      printf(" ps '%s'\n", sp);
   }
   return 0;
}
