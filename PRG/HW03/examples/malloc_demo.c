#include <stdlib.h>
#include <stdio.h>

void* mem_alloc(size_t size);
void mem_release(void **ptr);
void fill_array(int size, int* array);

int main(int argc, char *argv[]) 
{
   int *int_array;
   const int size = 4;

   int_array = mem_alloc(sizeof(int) * size);

   fill_array(size, int_array);

   int *cur = int_array;
   for (int i = 0; i < size; ++i, cur++) {
      printf("Array[%d] = %d\n", i, *cur);
   }

   mem_release((void**)&int_array);
   return 0;
}

void* mem_alloc(size_t size)
{
   void *ptr = malloc(size);
   if (ptr == NULL) {
      fprintf(stderr, "Error: required block of memory has not been allocated!\n");
      exit(-1); /* if allocation fail exit the program */
   }
   return ptr;
}

void fill_array(int size, int* array) 
{
   /* array is a local variable initialized 
    * with the calling value */
   /* notice size may be larger than int */
   for (int i = 0; i < size; ++i) {
      *(array++) = random() % 10; // pointer arithmetic
      //array[i] = random() % 10; // or use array notation
   }
}

void mem_release(void **ptr)
{
   if (ptr != NULL && *ptr != NULL) {
      free(*ptr);
      *ptr = NULL;
   }
}
