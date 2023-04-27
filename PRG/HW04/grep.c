#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INIT_SIZE 10
#define PATTERN_POS 1
#define FILE_POS 2
#define OK 0
#define NUM_ARGS_ERROR 100
#define NUM_FUNCTIONALS 2
#define REGULAR_EXPRESSION 0
#define COLORING_OUTPUT 1
#define NO_PATTERN 101
#define MALLOC_ERROR -1
#define NO_INPUT_FILE -1
#define NUM_REG_TYPES 3
#define NO_MATCH 1

char **load_input(FILE *input_file, int *num_lines);
char *load_line(FILE *input_stream);
void free_2d_arr(char **arr, int num_lines);
int set_prg_mode(int argc, char *argv[], int *file_name_index, int *pattern_idx, bool *regular_expression, bool *color_match);
bool compare(const char *str1, const char *str2);
int str_len(const char *str);
char *copy_string(const char *str);
bool match(char **input, const char *pattern, int num_lines, bool color_match);
int question_reg(const char *str, const char letter, int index);
int star_reg(const char *str, const char letter, int index);
int plus_reg(const char *str, const char letter, int index);
int get_reg_type(char letter);
int *regex(const char *str, const char *pattern, int *indexes_len);
void color_print(int *indexes, int num_indexes, const char *str);
int cmpfunc(const void *a, const void *b);
int bonus_pattern_shift(const char *pattern, const char letter, int index);

const char *functionals[] = {"-E", "--color=always"};
const char reg_types[] = "?*+";

int (*regular_expressions[3])(const char *, const char, int) = {question_reg, star_reg, plus_reg};

/* The main program */
int main(int argc, char *argv[])
{
  FILE *input_file = NULL;
  int num_lines = 0, err = OK, file_name_index = NO_INPUT_FILE, pattern_index;
  char **input = NULL;
  bool regular_expresion = false, color_match = false, non_empty_input = false;
  char *pattern;
  // reads out functionals from program call and sets up grep mode
  err = set_prg_mode(argc, argv, &file_name_index, &pattern_index, &regular_expresion, &color_match);
  if (!err)
  {
    // loads pattern and input file / stdin
    pattern = argv[pattern_index];
    if (file_name_index != NO_INPUT_FILE)
      input_file = fopen(argv[file_name_index], "r");

    input = load_input(input_file, &num_lines);

    if (input_file != NULL)
      fclose(input_file);
    // calls function to match each line with pattern
    non_empty_input = match(input, pattern, num_lines, color_match);
  }

  // if there is no match - return value = 1
  if (!non_empty_input)
    err = NO_MATCH;

  free_2d_arr(input, num_lines);
  return err;
}

void color_print(int *indexes, int num_indexes, const char *str)
{
  // prints out line colored where there was match with pattern
  int index = 0, match_index = 0;
  while (str[index] != '\0')
  {
    if (match_index < num_indexes)
    {
      if (index == indexes[match_index] && (match_index % 2) == 0)
      {
        printf("\033[01;31m\033[K");
        match_index++;
      }

      putchar(str[index]);

      if (index == indexes[match_index])
      {
        printf("\033[m\033[K");
        match_index++;
      }
      index++;
    }
    else
    {
      putchar(str[index++]);
    }
  }
}

int set_prg_mode(int argc, char *argv[], int *file_name_index, int *pattern_idx, bool *regular_expression, bool *color_match)
{
  // first argument is name of the program -> skip it
  // reads out arguments which was program called with
  int num_functionals = 0, pattern_index, err = OK;
  for (int i = 1; i < argc; ++i)
  {
    char *arg = argv[i];
    for (int i = 0; i < NUM_FUNCTIONALS; ++i)
    {
      if (compare(functionals[i], arg))
      {
        if (i == REGULAR_EXPRESSION)
          *regular_expression = true;
        else if (i == COLORING_OUTPUT)
          *color_match = true;
        num_functionals++;
      }
    }
  }
  pattern_index = 1 + num_functionals;
  if (pattern_index >= argc)
    err = NO_PATTERN;
  else
  {
    *pattern_idx = pattern_index++;
    if (pattern_index < argc)
      *file_name_index = pattern_index;
  }
  return err;
}

int str_len(const char *str)
{
  // returns len of string (expects '\0' at the end)
  int s_len = -1;
  while (str[++s_len] != '\0')
    ;
  return s_len;
}

int cmpfunc(const void *a, const void *b)
{
  // function used in <stdlib> qsort for comparing 2 numbers
  return (*(int *)a - *(int *)b);
}

bool match(char **input, const char *pattern, int num_lines, bool color_match)
{
  // goes through each line and tries to find all matches with pattern
  bool non_empty_input = false;
  for (int i = 0; i < num_lines; ++i)
  {
    int num_indxes = 0;
    int *index_array = NULL;
    index_array = regex(input[i], pattern, &num_indxes);
    if (num_indxes == 0)
    {
      free(index_array);
      continue;
    }
    non_empty_input = true;
    qsort(index_array, num_indxes, sizeof(int), cmpfunc);
    if (color_match)
    {
      color_print(index_array, num_indxes, input[i]);
    }
    else
    {
      printf("%s", input[i]);
    }
    free(index_array);
  }
  return non_empty_input;
}

int *regex(const char *str, const char *pattern, int *indexes_len)
{
  // goes through line backwards -> easier for regular expressions
  // returns array of start and end indexes of whole pattern match
  int string_len = str_len(str) - 1;
  int pattern_len = str_len(pattern);
  int array_size = INIT_SIZE;
  int *indexes = (int *)malloc(array_size * sizeof(int));
  if (indexes == NULL)
  {
    fprintf(stderr, "Malloc error\n");
    exit(-1);
  }
  // printf("string len: %d, pattern len: %d, s letter %c, p letter %c \n", string_len, pattern_len, str[string_len - 1], pattern[pattern_len - 1]);
  for (int i = string_len - 1; i >= 0; --i)
  {
    int counter_pattern = 0, counter_string = 0;
    int index_end = i, index_len = 0;
    while (true)
    {
      if (str[i - counter_string] == pattern[pattern_len - 1 - counter_pattern])
      {
        counter_pattern++;
        counter_string++;
        index_len++;
      }
      else
      {
        int reg_type = get_reg_type(pattern[pattern_len - 1 - counter_pattern]);
        if (!reg_type)
          break;
        char reg_letter = pattern[pattern_len - 2 - counter_pattern];
        int index_offset = (*regular_expressions[reg_type - 1])(str, reg_letter, i - counter_string);
        if (index_offset < 0)
          break;
        counter_pattern += 2;
        if(reg_type != 1)
          counter_pattern += bonus_pattern_shift(pattern, reg_letter, pattern_len - 1 - counter_pattern);
        counter_string += index_offset;
        index_len += index_offset;
      }
      if (pattern_len - 1 - counter_pattern < 0)
      {
        // save the results
        // start and end index of match
        indexes[(*indexes_len)++] = index_end + 1 - index_len;
        indexes[(*indexes_len)++] = index_end;
        if ((*indexes_len) >= array_size)
        {
          array_size *= 2;
          int *tmp = (int *)realloc(indexes, array_size * sizeof(int));
          if (tmp != NULL)
          {
            indexes = tmp;
            tmp = NULL;
          }
          else
          {
            fprintf(stderr, "Realloc Error\n");
            exit(-1);
          }
        }
        break;
      }
      if (i == 0)
        break;
    }
  }
  return indexes;
}

int get_reg_type(char letter)
{
  // returns what regular_expression type should by used
  // or 0 if none
  int reg_type = 0;
  for (int i = 0; i < NUM_REG_TYPES; ++i)
  {
    if (reg_types[i] == letter)
    {
      reg_type = i + 1;
      break;
    }
  }
  return reg_type;
}

int bonus_pattern_shift(const char *pattern, const char letter, int index)
{
  int shift = 0;
  while (index >= 0 && pattern[index--] == letter)
  {
    shift++;
  }
  return shift;
}

int question_reg(const char *str, const char letter, int index)
{
  // used when ? is used in pattern
  if (str[index] == letter)
  {
    return 1;
  }
  return 0;
}

int star_reg(const char *str, const char letter, int index)
{
  // used when * is used in pattern
  if (str[index--] != letter)
    return 0;
  int match_len = 1;
  while (index >= 0 && str[index--] == letter)
  {
    match_len++;
  }
  return match_len;
}

int plus_reg(const char *str, const char letter, int index)
{
  // used if + is used in pattern
  if (str[index--] != letter)
    return -1;
  int match_len = 1;
  while (index >= 0 && str[index--] == letter)
  {
    match_len++;
  }
  return match_len;
}

bool compare(const char *str1, const char *str2)
{
  if (str_len(str1) != str_len(str2))
  {
    return false;
  }
  int string_len = str_len(str1);
  for (int i = 0; i < string_len; ++i)
  {
    if (str1[i] != str2[i])
      return false;
  }
  return true;
}

char **load_input(FILE *input_file, int *num_lines)
{
  // if there is no input file given -> read from stdin
  // loads input as 2D array
  if (input_file == NULL)
    input_file = stdin;

  int size = INIT_SIZE;
  char **input = (char **)malloc(size * sizeof(char *));
  if (input == NULL)
  {
    fprintf(stderr, "Memory allocation error\n");
    exit(-1);
  }
  char *line;
  line = load_line(input_file);
  while (line != NULL)
  {
    input[(*num_lines)++] = line;
    if ((*num_lines) == size)
    {
      size *= 2;
      char **tmp = realloc(input, size * sizeof(char *));
      if (tmp != NULL)
      {
        input = tmp;
        tmp = NULL;
      }
      else
      {
        fprintf(stderr, "Memory reallocation error\n");
        exit(-1);
      }
    }
    line = load_line(input_file);
  }
  return input;
}

char *load_line(FILE *input_stream)
{
  // loads single line from file/stdin
  char letter;
  int line_size = INIT_SIZE, line_len = 0;
  char *line = (char *)calloc(line_size, sizeof(char));
  if (line == NULL)
  {
    fprintf(stderr, "Memory allocation error\n");
    exit(-1);
  }
  letter = fgetc(input_stream);

  while (letter != -1)
  {
    line[line_len++] = letter;
    if (line_len == (line_size - 1))
    {
      line_size *= 2;
      char *tmp = realloc(line, line_size * sizeof(char));
      if (tmp != NULL)
      {
        line = tmp;
        tmp = NULL;
      }
      else
      {
        fprintf(stderr, "Mem Realloc ERROR");
      }
    }
    if (letter == '\n')
      break;
    letter = fgetc(input_stream);
  }
  if (line_len == 0)
  {
    // EOF must have occured
    free(line);
    line = NULL;
  }
  else
  {
    line[line_len] = '\0';
  }
  return line;
}

void free_2d_arr(char **arr, int num_lines)
{
  // frees whole dynamically allocated array
  for (int i = 0; i < num_lines; ++i)
  {
    free(arr[i]);
    arr[i] = NULL;
  }
  free(arr);
  arr = NULL;
}
