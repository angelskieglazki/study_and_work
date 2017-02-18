#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

#define TIMEOUT 5
#define BUF_LEN 1024

int main() {
  // registration event
  struct kevent change; // events we want to monitor
  struct kevent event;  // event that were triggered

  // create event queue
  int kq = kqueue();
  if (kq == -1) {
    perror("kqueue");
    return 1;
  }

  EV_SET(&change, STDIN_FILENO, EVFILT_READ, EV_ADD, 0, 0, NULL);

  struct timespec ts;
  ts.tv_sec = TIMEOUT;
  ts.tv_nsec = 0;

  while(1) {
    int nev = kevent(kq, &change, 1, &event, 1, &ts);
    if (new < 0) {
      perror("kevent");
      return 1;
    }
    if (nev == 0) {
      printf("%d seconds elapsed.\n", TIMEOUT);
      close(kq);
      return 0;
    }
    if (event.ident == STDIN_FILENO) {
      char buf[BUF_LEN + 1];

      int len = read(STDIN_FILENO, buf, BUF_LEN);
      if (len == -1) {
        perror("read");
        close(kq);
        return 1;
      } else if (len) {
        buf[len] = '\0';
        printf("read: %s\n", buf);
        close(kq);
        return 0;
      }
    }
  }
  close(kq);
  return 0;
}
