#include <stdio.h>

int main(void)
{
   char str1[] = "BE5B99PRG";
   char *str2 = "BE5B99PRG";

   printf("str1 \"%s\"\n", str1);
   printf("str2 \"%s\"\n", str2);

   printf("size of str1 %u\n", sizeof(str1));
   printf("size of str2 %u\n", sizeof(str2));

   return 0;

}
