#include "apue.h"

int main(void) {
    struct stat statbuf;

    if (stat("foo", &statbuf) < 0)
        err_sys("Error to call stat for foo file");
    if (chmod("foo", (statbuf.st_mode & S_IXGRP) | S_ISGID|S_IWUSR) < 0)
        err_sys("Error to call chmod for foo file");

    if (chmod("bar", S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH) < 0)
        err_sys("Error to call chmod for bar file");

    exit(0);        
}