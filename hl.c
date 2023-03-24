#include <stdio.h>
#include <string.h>
#include <regex.h>

#define ESC_COLOR_INDEX_MAX 6

typedef struct hl_option {
  int cflags;
  int color_index;
  int is_bright;
} hl_option_t;

int esc_colors[ESC_COLOR_INDEX_MAX] = {
  31, // red
  32, // green
  33, // yello
  34, // blue
  35, // magenta
  36, // cyan
};

int process_option(char option, hl_option_t *hl_opt) {
  int result = -1;
  switch (option) {
    case 'i' : // ignore string case
      hl_opt->cflags |= REG_ICASE;
      break;
    case 'E' : // support extended
      hl_opt->cflags |= REG_EXTENDED;
      break;
    case 'c' : // change color index
      hl_opt->color_index += 1;
      hl_opt->color_index = hl_opt->color_index % ESC_COLOR_INDEX_MAX;
      break;
    case 'b' : // set bright text color
      hl_opt->is_bright = 1;
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
      hl_opt->color_index = option - '0';
      break;
    case 'h' : // print help
      printf(
          "i : ignore upper/lower case\n"
          "E : support extended regex\n"
          "h : help\n"
          "");
      result = 0; // normal option but need to quit
      break;
    default :
      fprintf(stderr, "unknown option %c \n", option);
      result = 1;
      break;
  }
  return result;
}

void print_esc(int code)
{
  printf("\033[%dm", code);
}

int parse_options(char *options, hl_option_t *hl_opt) {
  int result = -1;
  for (int i = 1; i < strlen(options); i ++) {
    result = process_option(options[i], hl_opt);
    if (result != -1) {
      break;
    }
  }
  return result;
}

int main(int argc, char *argv[]) {
  char buffer[1024];
  char *pattern = NULL;
  regex_t regex;
  int cflags = 0;
  hl_option_t hl_opt = {0,};

  if (argc < 2) {
    fprintf(stderr, "argument not founded\n");
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    if(argv[i][0] == '-') {
      int result = parse_options(argv[i], &hl_opt);
      if (result != -1) {

        return result;
      }
    }
    else {
      pattern = argv[i];
      break; // ignore after arguments
    }
  }

  if (pattern == NULL) {
    fprintf(stderr, "pattern not found\n");
    return 1;
  }

  if (regcomp(&regex, pattern, hl_opt.cflags) != 0) {
    fprintf(stderr, "regex compilation error\n");
    return 1;
  }

  while (fgets(buffer, sizeof(buffer), stdin)) {
    int start = 0;
    regmatch_t match;

    while (regexec(&regex, buffer + start, 1, &match, 0) == 0) {
      int match_start = match.rm_so + start;
      int match_end = match.rm_eo + start;

      print_esc(1); // bold

      for (int i = start; i < match_start; i++) {
        putchar(buffer[i]);
      }

      print_esc(
        esc_colors[hl_opt.color_index] +
        (hl_opt.is_bright == 1?60:0)
      );

      for (int i = match_start; i < match_end; i++) {
        putchar(buffer[i]);
      }
      print_esc(0); // reset
      print_esc(1); // bold

      start = match_end;
    }

    for (int i = start; i < strlen(buffer); i++) {
      if (i == strlen(buffer) -1) {
        print_esc(0); // reset
      }
      putchar(buffer[i]);
    }

  }

  regfree(&regex);

  return 0;
}

