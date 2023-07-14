#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ESC_COLOR_INDEX_MAX 6

typedef struct hl_option {
  int cflags;
  int color_index;
  int is_bright;
  char *pattern;
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
  case 'w': // capture warning
    hl_opt->cflags |= REG_ICASE;
    hl_opt->cflags |= REG_EXTENDED;
    hl_opt->pattern = "warn|warning";
    hl_opt->color_index = 2;
    break;
  case 'e': // capture error
    hl_opt->cflags |= REG_ICASE;
    hl_opt->cflags |= REG_EXTENDED;
    hl_opt->pattern = "err|error|fail|abort";
    hl_opt->color_index = 0;
    break;
  case 'o': // capture error
    hl_opt->cflags |= REG_ICASE;
    hl_opt->cflags |= REG_EXTENDED;
    hl_opt->pattern = "ok|good|nice|fine|success";
    hl_opt->color_index = 1;
    break;
  case 'i': // ignore string case
    hl_opt->cflags |= REG_ICASE;
    break;
  case 'E': // support extended
    hl_opt->cflags |= REG_EXTENDED;
    break;
  case 'c': // change color index
    hl_opt->color_index += 1;
    hl_opt->color_index = hl_opt->color_index % ESC_COLOR_INDEX_MAX;
    break;
  case 'b': // set bright text color
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
  case 'h': // print help
    printf("i : ignore upper/lower case\n"
           "E : support extended regex\n"
           "c : change to next color index\n"
           "0~5 : change color index\n"
           "h : help\n"
           "");
    result = 0; // normal option but need to quit
    break;
  default:
    fprintf(stderr, "unknown option %c \n", option);
    result = 1;
    break;
  }
  return result;
}

void print_esc(int code) { printf("\033[%dm", code); }

int parse_options(char *options, hl_option_t *hl_opt) {
  int result = -1;
  for (int i = 1; i < strlen(options); i++) {
    result = process_option(options[i], hl_opt);
    if (result != -1) {
      break;
    }
  }
  return result;
}

int main(int argc, char *argv[]) {
  char *buffer = NULL;
  size_t bufferLen = 0;
  ssize_t read;
  regex_t regex;
  int cflags = 0;
  hl_option_t hl_opt = {
      0,
  };
  hl_opt.pattern = NULL;

  if (argc < 2) {
    fprintf(stderr, "argument not founded\n");
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      int result = parse_options(argv[i], &hl_opt);
      if (result != -1) {

        return result;
      }
    } else if (hl_opt.pattern == NULL) {
      hl_opt.pattern = argv[i];
      break; // ignore after arguments
    }
  }

  if (hl_opt.pattern == NULL) {
    fprintf(stderr, "pattern not found\n");
    return 1;
  }

  if (regcomp(&regex, hl_opt.pattern, hl_opt.cflags) != 0) {
    fprintf(stderr, "regex compilation error\n");
    return 1;
  }

  while ((read = getline(&buffer, &bufferLen, stdin)) != -1) {
    int start = 0;
    regmatch_t match;

    while (regexec(&regex, buffer + start, 1, &match, 0) == 0) {
      int match_start = match.rm_so + start;
      int match_end = match.rm_eo + start;

      print_esc(1); // bold

      for (int i = start; i < match_start; i++) {
        putchar(buffer[i]);
      }

      print_esc(esc_colors[hl_opt.color_index] +
                (hl_opt.is_bright == 1 ? 60 : 0));

      for (int i = match_start; i < match_end; i++) {
        putchar(buffer[i]);
      }
      print_esc(0); // reset
      print_esc(1); // bold

      start = match_end;
    }

    for (int i = start; i < read; i++) {
      if (i == read - 1) {
        print_esc(0); // reset
      }
      putchar(buffer[i]);
    }

    fflush(stdout);
  }

  regfree(&regex);
  free(buffer);

  return 0;
}
