#include <stdio.h>

void main() {
  int i;
  if(fork()) {
    printf("parent\n");
  }else{
    printf("child\n");
  }
}
