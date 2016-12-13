#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[]) {
    
    if (argc != 2)
        err_quit("Usage: a.out <name of file>");
    
    if (access(argv[1], R_OK) < 0)
        err_ret("Error to call function access for %s", argv[1]);
    else
        printf("Access for read complete\n");
    
    if (open(argv[1], O_RDONLY) < 0)
        err_ret("Error to call function open for %s", argv[1]);
    else
    printf("File open for read");

    exit(0);
}