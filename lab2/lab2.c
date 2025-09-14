#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {

  char line[1024];
  puts("Enter programs to run.");

  for (;;) {
    fputs("> ", stdout);
    fflush(stdout);

    if (!fgets(line, sizeof line, stdin)) {
      putchar('\n');
      break;
    }

    size_t n = strlen(line);
    if (n && line[n - 1] == '\n') {
      line[--n] = '\0';
    }
    char *save = NULL;
    char *path = strtok_r(line, " \t", &save);

    if (!path) {
      continue;
    }

    pid_t pid = fork();
    if (pid < 0) {
      perror("fork");
      continue;
    }

    if (pid == 0) {
      execl(path, path, (char *)NULL);

      (void)errno;
      fprintf(stderr, "Exec failure\n");
      _exit(127);
    }

    int status;
    if (waitpid(pid, &status, 0) < 0) {
      perror("waitpid");
    }
  }

  return 0;
}
