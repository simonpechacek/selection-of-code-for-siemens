#include <stdio.h>
#include <string.h>

int getLength(char *str);

int main(int argc, char *argv[]) 
{
   for (int i = 0; i < argc; ++i) {
      printf("argv[%i]: getLength = %i -- strlen = %lu\n", i, getLength(argv[i]), strlen(argv[i]));
   }
   return 0;
}

int getLength(char *str)
{
   int ret = 0;
   while (str && (*str++) != '\0') {
      ret++;
   }
   return ret;
}

