#include "s21_cat.h"

int main(int argc, char *argv[]) {
  struct flags current_flags = {0};
  int correct_flags = parser(argc, argv, &current_flags);
  int res = 0;

  if (correct_flags) {
    for (int i = optind; i < argc; i++) {
      reader(argv[i], &current_flags);
    }
  } else {
    printf("usage: cat [-belnstuv] [file ...]\n");
    res = 1;
  }

  return res;
}

int parser(int argc, char *argv[], struct flags *current_flags) {
  int correct_flag = 1;
  int result = 0;
  while ((result = getopt_long(argc, argv, "+benstvET", long_flags, NULL)) !=
             -1 &&
         correct_flag) {
    switch (result) {
      case 'b':
        current_flags->b = 1;
        break;
      case 'e':
        current_flags->e = 1;
        current_flags->v = 1;
        break;
      case 'n':
        current_flags->n = 1;
        break;
      case 's':
        current_flags->s = 1;
        break;
      case 't':
        current_flags->t = 1;
        current_flags->v = 1;
        break;
      case 'T':
        current_flags->T = 1;
        break;
      case 'E':
        current_flags->E = 1;
        break;
      default:
        correct_flag = 0;
        break;
    }
  }
  return correct_flag;
}

void reader(char *filename, struct flags *current_flags) {
  FILE *file = fopen(filename, "r");

  if (file != NULL) {
    cat(file, current_flags);
    fclose(file);
  } else {
    printf("cat: %s: No such file or directory\n", filename);
  }
}

void cat(FILE *file, struct flags *current_flags) {
  int ch;
  int new_line = 1;
  int line_count = 1;
  int empty_line = 0;

  while ((ch = fgetc(file)) != EOF) {
    if (current_flags->b)
      b_flag(ch, &new_line, &line_count);
    else if (current_flags->E)
      E_flag(ch);
    else if (current_flags->n)
      n_flag(ch, &line_count, &new_line);
    else if (current_flags->s)
      s_flag(ch, &empty_line);
    else if (current_flags->T)
      T_flag(ch);
    else if (current_flags->e)
      e_flag(ch);
    else if (current_flags->t)
      t_flag(ch);
    else if (are_all_flags_zero(current_flags))
      default_cat(ch);
  }
}

void b_flag(int ch, int *new_line, int *line_count) {
  if (*new_line == 1 && ch != '\n') {
    printf("%6d  ", *line_count);
    *new_line = 0;
    (*line_count)++;
  }
  if (ch == '\n') {
    *new_line = 1;
  }
  printf("%c", ch);
}

void E_flag(int ch) {
  if (ch == '\n') {
    printf("$\n");
  } else {
    printf("%c", ch);
  }
}

void n_flag(int ch, int *line_count, int *new_line) {
  if (*new_line) {
    printf("%6d  ", *line_count);
    (*line_count)++;
    *new_line = 0;
    if (ch == 10) {
      *new_line = 1;
    }
  } else if (ch == 10) {
    *new_line = 1;
  }
  printf("%c", ch);
}

void s_flag(int ch, int *empty_line) {
  if (ch == '\n') {
    (*empty_line)++;
    if (*empty_line <= 2) {
      printf("%c", ch);
    }
  } else if (ch != '\n') {
    *empty_line = 0;
    printf("%c", ch);
  }
}

void T_flag(int ch) {
  if (ch != '\t') {
    printf("%c", ch);
  } else {
    printf("^I");
  }
}

void e_flag(int ch) {
  if (ch == 10) {
    printf("$");
  } else if (ch == 9) {
    printf("\t ");
  } else if (ch < 32 || ch == 127) {
    printf("^");
    ch += 64;
  } else if (ch > 127) {
    printf("M-");
    ch -= 64;
  }
  printf("%c", ch);
}

void t_flag(int ch) {
  if (ch == 9) {
    printf("^I");
  } else if ((ch < 32 || ch == 127) && ch != 10) {
    printf("^");
    ch += 64;
  } else if (ch > 127) {
    printf("M-");
    ch -= 64;
  }
  printf("%c", ch);
}

int are_all_flags_zero(struct flags *f) {
  return (f->b == 0 && f->e == 0 && f->n == 0 && f->s == 0 && f->t == 0 &&
          f->T == 0 && f->E == 0 && f->v == 0);
}

void default_cat(int ch) { printf("%c", ch); }