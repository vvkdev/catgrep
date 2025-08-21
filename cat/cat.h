#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdio.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} st_flags;

void set_flags(const int opt, st_flags *fl, int *err);
void output(FILE *f, st_flags *fl, int *n, int *prev, int *first, int *cnt);
int show_nonprint(int ch);

#endif
