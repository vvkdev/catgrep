#define _GNU_SOURCE

#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int o;
  int f;
} st_flags;

void set_flags(int opt, st_flags *fl, char *str, int *err);
int read_patterns_from_file(char *filename, char *str);
void grep(char *filename, st_flags *fl, char *str, int files);
void output(FILE *f, char *filename, st_flags *fl, regex_t *reg, int files);
void print_no_file(char *filename);
void replace(char *str, const char *old_str, const char *new_str);

#endif
