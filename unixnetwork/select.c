#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define TIMEOUT 5
#define BUF_LEN 1024

int main() {
    struct timeval tv;
    tv.tv_sec   = TIMEOUT;
    tv.tv_usec  = 0;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
    if (ret == -1) {
        perror("select");
        return 1;
    } else if (!ret) {
        printf("%d seconds elapsed\n", TIMEOUT);
        return 0;
    }

    if (FD_ISSET(STDIN_FILENO, &readfds)) {
        char buf[BUF_LEN+1];
        int len = read(STDIN_FILENO, buf, BUF_LEN);
        if (len == -1) {
            perror("read");
            return 1;
        } else if (len) {
            buf[len] = '\0';
            printf("read: %s\n", buf);
        }
        return 0;
    }

    fprintf(stderr, "It is impossible!\n");
    return 1;
}
