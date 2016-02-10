#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <limits.h>

/* Buffer for 100 events */
#define BUFSIZE(100*(sizeof(struct inotify_event)+NAME_MAX+1))

void main() {
  
  int notifyfd, watchfd;
  char eventbuf[BUFSIZE];
  int n;
  char *p;
  struct inotify_event *event;
  
  notifyfd = inotify_init();
  watchfd = inotify_add_watch(notifyfd, ".", IN_CREATE|INDELETE);

  while(1) {
    n = read(notifyfd, eventbuf, BUFSIZE);
    for (p = eventbuf; p < eventbuf+n;) {
      event = (struct inotify_event *)p;
      p +=sizeof(struct inotify_event) + event->len;
      if (event->mask & IN_CREATE) printf("%s create\n", event->name);
      if (event->mask & IN_DELETE) printf("%s delete\n", event->name);
    }
  }
  return 0;
}
