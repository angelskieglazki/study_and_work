#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  char* argv[] = { "mcedit", "exec-1.c", NULL };

  printf("main process, %jd\n", (intmax_t)getpid());
  // execl
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) { // child process
    printf("child process, %jd\n", (intmax_t)getpid());
    if (execl("/usr/bin/terminator", "terminator", NULL) < 0) {
      perror("execl");
      exit(EXIT_FAILURE);
    }
  } else if (pid > 0) { // parent process
    printf("parent of process, %jd\n", (intmax_t)pid);
  }

  if (waitpid(pid, NULL, 0) < 0) {
    perror("waitpid");
    exit(EXIT_FAILURE);
  } else {
    printf("waitpid succeeded, %jd\n", (intmax_t)getpid());
  }

  printf("main process, %jd\n", (intmax_t)getpid());
  pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) { // child process
    printf("child process, %jd\n", (intmax_t)getpid());
    if (execlp("env", "env", NULL) < 0) {
      perror("execlp");
      exit(EXIT_FAILURE);
    }
  }

  if (waitpid(pid, NULL, 0) < 0) {
    perror("waitpid");
    exit(EXIT_FAILURE);
  } else {
    printf("waitpid succeeded, %jd\n", (intmax_t)getpid());
  }

  return EXIT_SUCCESS;
}