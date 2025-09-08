#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  // TODO: read a line using getliine()
  char *buf = NULL;
  size_t cap = 0;
  ssize_t nread;

  printf("Enter a line: ");
  nread = getline(&buf, &cap, stdin);

  if (nread < 0) {
    perror("getline");
    return 1;
  }

  if (nread > 0 && buf[nread - 1] == '\n') {
    buf[nread - 1] = '\0';
  }
  // TODO: tokenize with strtok_r()
  char *saveptr = NULL;
  char *tok = strtok_r(buf, " ", &saveptr);

  // TODO: print each token
  while (tok != NULL) {
    printf("Token: %s\n", tok);
    tok = strtok_r(NULL, " ", &saveptr);
  }

  free(buf); // clean up memory
  return 0;
}
