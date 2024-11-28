#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

struct flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int T;
  int E;
  int v;
};

const struct option long_flags[] = {{"number-nonblank", 0, NULL, 'b'},
                                    {"number", 0, NULL, 'n'},
                                    {"squeeze-blank", 0, NULL, 's'},
                                    {NULL, 0, NULL, 0}};

int parser(int argc, char *argv[], struct flags *current_flags);
void reader(char *filename, struct flags *current_flags);
void cat(FILE *file, struct flags *current_flags);
void b_flag(int ch, int *new_line, int *line_count);
void E_flag(int ch);
void n_flag(int ch, int *line_count, int *new_line);
void s_flag(int ch, int *empty_line);
void T_flag(int ch);
void e_flag(int ch);
void t_flag(int ch);
int are_all_flags_zero(struct flags *f);
void default_cat(int ch);

#endif