#include "cat.h"

int main(int argc, char *argv[]) {
  int err = 0;
  if (argc > 1) {
    int prev = 0, n = 1, opt = 0, i = 0, first = 1, cnt = 0;
    st_flags fl = {0};
    static struct option long_fl[] = {
        {"number-nonblank", 0, 0, 'b'},
        {"number", 0, 0, 'n'},
        {"squeeze-blank", 0, 0, 's'},
        {"show-nonprinting", 0, 0, 'v'},
        {0, 0, 0, 0},
    };

    opterr = 0;
    while (!err &&
           (opt = getopt_long(argc, argv, "beEnstTv", long_fl, &i)) != -1) {
      set_flags(opt, &fl, &err);
    }

    if (!err) {
      while (optind < argc) {
        FILE *f = fopen(argv[optind], "r");
        if (f) {
          output(f, &fl, &n, &prev, &first, &cnt);
          fclose(f);
        } else {
          fprintf(stderr, "cat: %s: No such file or directory\n", argv[optind]);
        }
        optind++;
      }
    }
  }
  return err;
}

void set_flags(const int opt, st_flags *fl, int *err) {
  switch (opt) {
    case 'b':
      fl->b = 1;
      fl->n = 0;
      break;
    case 'e':
      fl->e = 1;
      fl->v = 1;
      break;
    case 'E':
      fl->e = 1;
      break;
    case 'n':
      if (!fl->b) fl->n = 1;
      break;
    case 's':
      fl->s = 1;
      break;
    case 't':
      fl->t = 1;
      fl->v = 1;
      break;
    case 'T':
      fl->t = 1;
      break;
    case 'v':
      fl->v = 1;
      break;
    default:
      fprintf(stderr, "cat: invalid option\n");
      fprintf(stderr, "Try 'cat --help' for more information.\n");
      *err = 1;
      break;
  }
}

void output(FILE *f, st_flags *fl, int *n, int *prev, int *first, int *cnt) {
  int ch;
  while ((ch = fgetc(f)) != EOF) {
    if (fl->s && ch == '\n' && (*prev == '\n' || *first == 1))
      *cnt += 1;
    else
      *cnt = 0;

    if (*cnt <= 1) {
      if (fl->b && ch != '\n' && (*prev == '\n' || *n == 1))
        printf("%6d\t", (*n)++);
      if (fl->n && (*prev == '\n' || *n == 1)) printf("%6d\t", (*n)++);
      if (fl->e && ch == '\n') printf("$");
      if (fl->t && ch == '\t') printf("^"), ch = 'I';
      if (fl->v) ch = show_nonprint(ch);

      printf("%c", ch);
    }
    *prev = ch;
    *first = 0;
  }
}

int show_nonprint(int ch) {
  switch (ch) {
    case 0 ... 8:
    case 11 ... 31:
      printf("^");
      ch += 64;
      break;
    case 127:
      printf("^");
      ch -= 64;
      break;
    case 128 ... 159:
      printf("M-^");
      ch -= 64;
      break;
    case 160 ... 254:
      printf("M-");
      ch += 128;
      break;
    case 255:
      printf("M-^");
      ch = '?';
      break;
  }
  return ch;
}
