#include <stdio.h>
#include <unistd.h>
#include <poll.h>

#define TIMEOUT 5
#define BUF_LEN 1024

int main() {
    struct pollfd fds;

    fds.fd = STDIN_FILENO;
    fds.events = POLLIN;

    int ret = poll(&fds, 1, TIMEOUT * 1000);
    if (ret == -1) {
        perror("poll");
        return 1;
    }

    if (!ret) {
        printf("%d second elapsed.\n", TIMEOUT);
        return 0;
    }

    if (fds.revents & POLLIN) {
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
    return 0;
}
