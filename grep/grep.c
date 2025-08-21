#include "grep.h"

int main(int argc, char *argv[]) {
  int err = 0;
  if (argc > 1) {
    st_flags fl = {0};
    char str[1024] = {0};
    int opt = 0;

    opterr = 0;
    while (!err && (opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
      set_flags(opt, &fl, str, &err);
    }
    if (fl.v && fl.o) err = 1;

    if (!err && argv[optind] != NULL) {
      if (str[0] == '\0') strcat(str, argv[optind++]);
      if (!fl.l && !fl.c && fl.o) {
        replace(str, "||", "|");
        int last = strlen(str) - 1;
        if (str[last] == '|') str[last] = '\0';
      }

      int files = argc - optind;
      while (optind < argc) {
        grep(argv[optind], &fl, str, files);
        optind++;
      }
    }
  }
  return 0;
}

void set_flags(int opt, st_flags *fl, char *str, int *err) {
  switch (opt) {
    case 'e':
      fl->e = 1;
      if (str[0] != '\0') strcat(str, "|");
      strcat(str, optarg);
      break;
    case 'i':
      fl->i = 1;
      break;
    case 'v':
      fl->v = 1;
      break;
    case 'c':
      if (!fl->l) fl->c = 1;
      break;
    case 'l':
      fl->l = 1;
      fl->c = 0;
      break;
    case 'n':
      fl->n = 1;
      break;
    case 'h':
      fl->h = 1;
      break;
    case 's':
      fl->s = 1;
      break;
    case 'f':
      fl->f = 1;
      *err = read_patterns_from_file(optarg, str);
      break;
    case 'o':
      fl->o = 1;
      break;
    default:
      fprintf(stderr, "grep: invalid option.\n");
      *err = 1;
      break;
  }
}

int read_patterns_from_file(char *filename, char *str) {
  int err = 0;
  FILE *f = fopen(filename, "r");
  if (f) {
    char *line = NULL;
    size_t len;
    while (getline(&line, &len, f) != -1) {
      int last = strlen(line) - 1;
      if (line[last] == '\n') line[last] = '\0';
      if (str[0] != '\0') strcat(str, "|");
      str = strcat(str, line);
    }
    if (line) free(line);
    fclose(f);
  } else {
    print_no_file(optarg);
    err = 1;
  }
  return err;
}

void grep(char *filename, st_flags *fl, char *str, int files) {
  regex_t reg;
  regcomp(&reg, str, fl->i ? REG_EXTENDED | REG_ICASE : REG_EXTENDED);

  FILE *f = fopen(filename, "r");
  if (f) {
    output(f, filename, fl, &reg, files);
    fclose(f);
  } else {
    if (!fl->s) print_no_file(filename);
  }
  regfree(&reg);
}

void output(FILE *f, char *filename, st_flags *fl, regex_t *reg, int files) {
  char *line = NULL;
  size_t len;
  int num = 0, cnt = 0;
  while (getline(&line, &len, f) != -1) {
    num++;
    regmatch_t regm = {0};
    int nomatch = regexec(reg, line, 1, &regm, 0);
    if (fl->v) nomatch = !nomatch;
    if (!nomatch) {
      ++cnt;
      if (files > 1 && !fl->h && !fl->l && !fl->c && !fl->o)
        printf("%s:", filename);
      if (!fl->l && !fl->c && fl->n && !fl->o) printf("%d:", num);
      if (!fl->l && !fl->c && fl->o) {
        char *tmp = line;
        while (nomatch == 0) {
          if (files > 1 && !fl->h) printf("%s:", filename);
          if (fl->n) printf("%d:", num);
          for (int i = regm.rm_so; i < regm.rm_eo; i++) printf("%c", tmp[i]);
          printf("\n");
          tmp = tmp + regm.rm_eo;
          nomatch = regexec(reg, tmp, 1, &regm, 0);
        }
      } else {
        if (!fl->l && !fl->c && !fl->o) printf("%s", line);
        if (!fl->l && !fl->c && line[strlen(line) - 1] != '\n') printf("\n");
      }
    }
  }
  if (fl->l && cnt > 0) printf("%s\n", filename);
  if (files > 1 && !fl->h && fl->c) printf("%s:", filename);
  if (!fl->l && fl->c) printf("%d\n", cnt);
  if (line) free(line);
}

void print_no_file(char *filename) {
  fprintf(stderr, "grep: %s: No such file or directory\n", filename);
}

void replace(char *str, const char *old_str, const char *new_str) {
  int old_len = strlen(old_str);
  int new_len = strlen(new_str);
  char *ptr = str;
  while ((ptr = strstr(ptr, old_str)) != NULL) {
    memmove(ptr + new_len, ptr + old_len, strlen(ptr + old_len) + 1);
    memcpy(ptr, new_str, new_len);
  }
}
