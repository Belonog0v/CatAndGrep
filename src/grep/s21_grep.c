#include "s21_grep.h"

int main(int argc, char *argv[]) {
  struct flags current_flags = {0};
  char pattern[INITIAL_BUFFER_SIZE] = {0};
  int correct_flags = 1;
  if (parser(argc, argv, &current_flags, pattern, &correct_flags))
    if (correct_flags) {
      for (int i = optind; i < argc; i++) {
        reader(argv[i], &current_flags, pattern, argc - optind);
      }
    }
  return 0;
}

int parser(int argc, char *argv[], struct flags *current_flags, char *pattern,
           int *correct_flag) {
  int result, flag = 1;
  if (argc <= 1) {
    *correct_flag = 0;
    flag = 0;
  }
  while ((result = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1 &&
         flag) {
    switch (result) {
      case 'e':
        current_flags->e = 1;
        combine_patterns(argc, argv, pattern);
        break;
      case 'i':
        current_flags->i = 1;
        break;
      case 'v':
        current_flags->v = 1;
        break;
      case 'c':
        current_flags->c = 1;
        break;
      case 'l':
        current_flags->l = 1;
        break;
      case 'n':
        current_flags->n = 1;
        break;
      case 'h':
        current_flags->h = 1;
        break;
      case 's':
        current_flags->s = 1;
        break;
      case 'f':
        current_flags->e = 1;
        if (*correct_flag) {
          if (read_patterns_in_file(optarg, pattern) == 0) flag = 0;
        }
        break;
      case 'o':
        current_flags->o = 1;
        break;
      default:
        *correct_flag = 0;
        break;
    }
  }
  if (!current_flags->e && !current_flags->f) {
    current_flags->e = 1;
    strcpy(pattern, argv[optind]);
    optind++;
  }

  return flag;
}

void reader(char *filename, struct flags *current_flags, char *pattern,
            int count_files) {
  FILE *file = fopen(filename, "r");
  int count_lines = 0, found_match = 0, number_line = 1, flag = 1;
  if (file == NULL) {
    if (!current_flags->s) perror(filename);
    flag = 0;
  }
  char *line = NULL;
  while (flag && (line = custom_getline(file)) != NULL) {
    processing(line, current_flags, pattern, &count_lines, &found_match,
               &number_line, count_files, filename);
    number_line++;
    free(line);
  }
  if (flag && current_flags->c) {
    if (count_files == 1)
      printf("%d\n", count_lines);
    else
      printf("%s:%d\n", filename, count_lines);
  }
  if (flag && current_flags->l && found_match) {
    printf("%s\n", filename);
  }
  if (flag) fclose(file);
}

void processing(char *line, struct flags *current_flags, char *pattern,
                int *count_lines, int *match_count, int *number_line,
                int count_files, char *filename) {
  int mode = REG_EXTENDED, flag = 1;
  if (current_flags->e) {
    regex_t regex;
    if (current_flags->i) {
      mode = REG_ICASE;
    }
    if (regcomp(&regex, pattern, mode) != 0) {
      printf("Compilation error!\n");
      flag = 0;
    }
    if (flag) {
      int proc = regexec(&regex, line, 0, NULL, 0);
      if (current_flags->v && proc) {
        if (current_flags->c) (*count_lines)++;
        if (current_flags->l) *match_count = 1;
        if (current_flags->n)
          print_flag_n(filename, line, *number_line, count_files);
        if (current_flags->h) printf("%s\n", line);
        if (!current_flags->c && !current_flags->l && !current_flags->n &&
            !current_flags->h && !current_flags->o)
          standart_print(filename, line, count_files);
      } else if (!current_flags->v) {
        if (current_flags->c && !proc) (*count_lines)++;
        if (current_flags->l && !proc) *match_count = 1;
        if (current_flags->n && !proc)
          print_flag_n(filename, line, *number_line, count_files);
        if (current_flags->h && !proc) printf("%s\n", line);
        if (current_flags->o)
          grep_o(line, pattern, count_files, filename, current_flags);
        if (!current_flags->c && !current_flags->l && !current_flags->n &&
            !current_flags->h && !current_flags->o && !proc)
          standart_print(filename, line, count_files);
      }
      regfree(&regex);
    }
  }
}

char *custom_getline(FILE *file) {
  int size = MAX_SIZE_LINE;
  char *buffer = (char *)malloc(size * sizeof(char));
  int i = 0, flag = 1;
  int c;
  if (!buffer) {
    flag = 0;
  }
  while ((c = fgetc(file)) != '\n' && c != EOF && flag) {
    buffer[i++] = (char)c;
    if (i >= size) {
      size *= 2;
      char *new_buffer = (char *)realloc(buffer, size * sizeof(char));
      if (!new_buffer) {
        free(buffer);
        buffer = NULL;
        flag = 0;
      }
      if (flag) buffer = new_buffer;
    }
  }
  if (flag) {
    buffer[i] = '\0';
    if (i == 0 && c == EOF) {
      free(buffer);
      buffer = NULL;
    }
  }
  return buffer;
}

int read_patterns_in_file(char *file_path, char *patterns_buffer) {
  FILE *file = fopen(file_path, "r");
  int flag = 1;
  if (file == NULL) {
    flag = 0;
  }
  if (flag) {
    patterns_buffer[0] = '\0';
    char *line = "";
    int first_pattern = 1;
    while ((line = custom_getline(file)) != NULL && flag) {
      line[strcspn(line, "\n")] = '\0';
      if (!first_pattern) {
        strcat(patterns_buffer, "|");
      } else {
        first_pattern = 0;
      }
      strcat(patterns_buffer, "(");
      strcat(patterns_buffer, line);
      strcat(patterns_buffer, ")");
      free(line);
    }
    fclose(file);
  }
  return flag;
}

void combine_patterns(int argc, char *argv[], char *patterns_buffer) {
  patterns_buffer[0] = '\0';
  int first_pattern = 1;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      continue;
    }
    if (!first_pattern) {
      strcat(patterns_buffer, "|");
    } else {
      first_pattern = 0;
    }
    strcat(patterns_buffer, "(");
    strcat(patterns_buffer, argv[i]);
    strcat(patterns_buffer, ")");
  }
}

void print_flag_n(char *filename, char *line, int number_line,
                  int count_files) {
  if (count_files == 1)
    printf("%d:%s\n", number_line, line);
  else
    printf("%s:%d:%s\n", filename, number_line, line);
}

void standart_print(char *filename, char *line, int count_files) {
  if (count_files == 1)
    printf("%s\n", line);
  else
    printf("%s:%s\n", filename, line);
}

void grep_o(char *line, char *pattern, int count_files, char *filename,
            struct flags *current_flags) {
  regex_t regex;
  regmatch_t match;
  int mode = REG_EXTENDED, flag = 1;
  if (current_flags->i) {
    mode = REG_ICASE;
  }
  if (regcomp(&regex, pattern, mode) != 0) {
    printf("Compilation error!\n");
    flag = 0;
  }
  if (flag && !current_flags->v) {
    while (regexec(&regex, line, 1, &match, 0) == 0) {
      int match_length = match.rm_eo - match.rm_so;
      if (!current_flags->c && !current_flags->l) {
        if (count_files == 1) {
          printf("%.*s\n", match_length, line + match.rm_so);
        } else {
          printf("%s:%.*s\n", filename, match_length, line + match.rm_so);
        }
      }
      line += match.rm_eo;
      if (*line == '\0') {
        break;
      }
    }
  }
  if (flag) regfree(&regex);
}