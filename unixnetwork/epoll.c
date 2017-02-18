#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

#define TIMEOUT 5
#define BUF_LEN 1024
#define MAX_EVENTS 64

int main() {
    int epfd = epoll_create1(0);
    if (epfd < 0) {
        perror("epoll_create1");
        return 1;
    }

    struct epoll_event event;
    event.data.fd = STDIN_FILENO;
    event.events = EPOLLIN;

    int epctl = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO,
            &event);
    if (epctl) {
        perror("epoll_ctl");
        return 1;
    }

    struct epoll_event *events = malloc(sizeof(struct epoll_event) * MAX_EVENTS);
    if (!events) {
        perror("malloc");
        return 1;
    }

    int nr_events = epoll_wait(epfd, events, MAX_EVENTS, TIMEOUT * 1000);
    if (nr_events < 0) {
        perror("epoll_wait");
        free(events);
        return 1;
    }

    if (!nr_events) {
        printf("%d seconds elapsed.\n", TIMEOUT);
        return 0;
    }

    for (int i = 0; i < nr_events; ++i) {
        if (events[i].events & EPOLLIN) {
            char buf[BUF_LEN + 1];

            int len = read(STDIN_FILENO, buf, BUF_LEN);
            if (len == -1) {
                perror("read");
                return 1;
            } else if (len) {
                buf[len] = '\0';
                printf("read: %s\n", buf);
            }
        }
    }

    free(events);

    return 0;
}




















