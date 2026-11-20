#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  // The environment comes from the launcher. getenv returns NULL for a name
  // that is not set.
  const char *name = getenv("USER");
  if (name == NULL) {
    name = getenv("NAME");
  }
  printf("Hello, %s, from Xila!\n", name != NULL ? name : "world");

  for (int i = 1; i < argc; ++i) {
    printf("arg: %s\n", argv[i]);
  }

  if (argc > 1 && strcmp(argv[1], "--fail") == 0) {
    fputs("Requested failure (--fail).\n", stderr);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
