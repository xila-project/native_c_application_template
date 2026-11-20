#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xila/stubs.h>

static const XilaProcess *process;

int32_t __real_xila_main(const XilaStartup *startup);

/* Keep the process capability for the environment lookup in main.
 * The SDK still initializes argc, argv and stdio and handles exit.
 */
int32_t __wrap_xila_main(const XilaStartup *startup) {
  process = startup->process;
  return __real_xila_main(startup);
}

int main(int argc, char **argv) {
  puts("Hello from Xila!");
  for (int i = 1; i < argc; ++i) {
    printf("arg: %s\n", argv[i]);
  }

  const char name[] = "XILA_GREETING";
  char value[64];
  size_t length;
  XilaStatus status = xila_env_get(process, name, sizeof(name) - 1, value,
                                   sizeof(value) - 1, &length);
  if (status == XILA_STATUS_OK) {
    value[length] = '\0';
    printf("%s=%s\n", name, value);
  } else if (status == XILA_STATUS_NOT_FOUND) {
    printf("%s is unset\n", name);
  } else {
    printf("%s is unavailable or exceeds 63 bytes\n", name);
  }

  return argc > 1 && strcmp(argv[1], "--fail") == 0 ? EXIT_FAILURE
                                                    : EXIT_SUCCESS;
}
