#include <stdio.h>
#include <string.h>
#include <regex.h>

#define HL_LINE "1"
#define HL_TEXT "31"

void process_option(char option, int *cflags) {
  switch (option) {
    case 'i' : // ignore string case
      *cflags |= REG_ICASE;
      break;
    case 'E' : // support extended
      *cflags |= REG_EXTENDED;
      break;
    default :
      break;
  }
}

void parse_options(char *options, int *cflags) {
  for(int i = 1; i < strlen(options); i ++) {
    process_option(options[i], cflags);
  }
}

int main(int argc, char *argv[]) {
  char buffer[1024];
  char *pattern = NULL;
  regex_t regex;
  int cflags = 0;

  if (argc < 2) {
    fprintf(stderr, "argument not founded\n");
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    if(argv[i][0] == '-') {
      parse_options(argv[i], &cflags);
    }
    else {
      pattern = argv[i];
      break; // ignore after arguments
    }
  }

  if (regcomp(&regex, pattern, cflags) != 0) {
    fprintf(stderr, "regex compilation error\n");
    return 1;
  }

  while (fgets(buffer, sizeof(buffer), stdin)) {
    int start = 0;
    regmatch_t match;

    while (regexec(&regex, buffer + start, 1, &match, 0) == 0) {
      int match_start = match.rm_so + start;
      int match_end = match.rm_eo + start;

      printf("\033[" HL_LINE "m");

      for (int i = start; i < match_start; i++) {
        putchar(buffer[i]);
      }

      printf("\033[1;" HL_TEXT "m");
      for (int i = match_start; i < match_end; i++) {
        putchar(buffer[i]);
      }
      printf("\033[0m");
      printf("\033[" HL_LINE "m");

      start = match_end;
    }

    for (int i = start; i < strlen(buffer); i++) {
      if (i == strlen(buffer) -1) {
        printf("\033[0m");
      }
      putchar(buffer[i]);
    }

  }

  regfree(&regex);

  return 0;
}

