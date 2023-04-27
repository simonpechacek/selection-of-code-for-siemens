#include <stdio.h>

int main(int argc, char *argv[]) 
{
   int v = 10;
   int v2 = 20;

   { /* pointer to a constant int */
      printf("\nPointer to a constant variable (value)\n");
      const int *ptr = &v; /* from right-to-left 
                              ptr -- variable that
                            *  -- is pointer to the variable 
                            const int -- of the 'const int' type
                            */  
      printf("*ptr: %d\n", *ptr);
      // *ptr = 11; /* cannot assigned */
      v = 11; /* the original variable can be stil modified */
      printf("*ptr: %d\n", *ptr);

      ptr = &v2; /* pointer can also be modified */
      printf("*ptr: %d\n", *ptr);
   }

   {  /* constant pointer */
      printf("\nConstant pointer\n");
      int *const ptr = &v;  /* from right-to-left 
                               ptr    -- variable that 
                             *const -- is constant pointer 
                             int    -- to the variable of the int type
                             */  
      v = 10;
      printf("v: %d *ptr: %d\n", v, *ptr);
      *ptr = 11; /* we can modify value at the address in the pointer */
      printf("v: %d\n", v);

      // ptr = &v2; /* we cannot set new address to the constant pointer */
   }

   {  /* constant pointer to a constant variable (value) */
      printf("\nConstant pointer\n");
      const int *const ptr = &v;  /* from right-to-left 
                                     ptr    -- variable that 
                                   *const -- is constant pointer 
                                   const int    -- to the variable of the const int type
                                   */  
      v = 10;
      printf("v: %d *ptr: %d\n", v, *ptr);
      // *ptr = 11; /* we cannot modify value at the address in the pointer */
      // ptr = &v2; /* we cannot set new address to the constant pointer */
   }
   return 0;
}
