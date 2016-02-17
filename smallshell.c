#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int parse_cmd_line(char **args) {
  static char line[100];

  printf(">");
  if(gets(line) == NULL) return -1;
  *args++ = strtok(line, "\t");
  while(*args++ = strtok(NULL, "\t"))
    ;
  return 1;
}

void main() {
  char*args[20];

  while(parse_cmd_line(args)>0) {
    if (fork() == 0) {
      execvp(args[0], args);
      printf("%s: not found\n", args[0]);
      exit(1);
    }else
      wait(0);
  }
}
