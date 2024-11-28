#ifndef S21_GREP_H
#define S21_GREP_H

#define _GNU_SOURCE
#define MAX_PATTERN_SIZE 1024
#define MAX_SIZE_LINE 1024
#define INITIAL_BUFFER_SIZE 1000000

struct flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
};

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parser(int argc, char *argv[], struct flags *current_flags, char *pattern,
           int *correct_flag);
void reader(char *filename, struct flags *current_flags, char *pattern,
            int count_files);
void processing(char *line, struct flags *current_flags, char *pattern,
                int *count_lines, int *match_count, int *number_line,
                int count_files, char *filename);
char *custom_getline(FILE *file);
int read_patterns_in_file(char *file_path, char *patterns_buffer);
void combine_patterns(int argc, char *argv[], char *patterns_buffer);
void print_flag_n(char *filename, char *line, int number_line, int count_files);
void standart_print(char *filename, char *line, int count_files);
void grep_o(char *line, char *pattern, int count_files, char *filename,
            struct flags *current_flags);
#endif