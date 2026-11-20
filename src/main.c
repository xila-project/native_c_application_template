#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  puts("Hello from Xila!");
  for (int i = 1; i < argc; ++i) {
    printf("arg: %s\n", argv[i]);
  }

  if (argc > 1 && strcmp(argv[1], "--fail") == 0) {
    fputs("Requested failure (--fail).\n", stderr);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
