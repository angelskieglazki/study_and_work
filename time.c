#include <stdio.h>
#include <time.h>

int main() {
  time_t t;
  t = time(NULL);
  struct tm *tm;
  tm = localtime(&t);
  if(tm->tm_hour >= 12 && tm->tm_wday == 2)
    printf("Tuesday afternoon\n");
  return 1;
}
