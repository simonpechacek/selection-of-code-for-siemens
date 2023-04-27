#include <stdio.h>

void print(int m[3][3], const char* name)
{
   printf("Matrix %s\n", name);
   for (int r = 0; r < 3; ++r) {
      for (int c = 0; c < 3; ++c) {
         printf("%4i", m[r][c]);
      }
      printf("\n");
   }
   printf("\n");
}

int main(void)
{
   int m0[3][3];
   int m1[3][3] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
   int m2[3][3] = { 1, 2, 3 }; //partial init, rest set to 0
   int m3[3][3] = { [0][0] = 1, [1][1] = 2, [2][2] = 3 }; //indexed init

   print(m0, "m0 -- not initialized");
   print(m1, "m1 -- initialized by rows, left to right");
   print(m2, "m2 -- partially initialized"); 
   print(m3, "m3 -- indexed initialization"); 
   return 0;
}
