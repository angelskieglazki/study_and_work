#include "apue.h"
#include <fcntl.h>

void set_fl(int fd, int flags) {
	int val;
	if ((val = fcntl(fd, F_GETFL, 0)) < 0)
		err_sys("error to call fcntl");
	switch (val & O_ACCMODE) {
		case O_RDONLY:
			printf("read only");
			break;
		case O_WRONLY:
			printf("write only");
			break;
		case O_RDWR:
			printf("read write");
			break;
		default:
			err_dump("unknown acces mode");	
	}
	if (val & O_APPEND)
		printf(", append");
	if (val & O_NONBLOCK)
		printf(", nonblocking");
	if (val & O_SYNC)
		printf(", synchronous writes");
	val |= flags;

	if (fcntl(fd, F_SETFL, val) < 0)
		err_sys("error to call fcntl");
	switch (val & O_ACCMODE) {
		case O_RDONLY:
			printf("read only");
			break;
		case O_WRONLY:
			printf("write only");
			break;
		case O_RDWR:
			printf("read write");
			break;
		default:
			err_dump("unknown acces mode");
	}
	if (val & O_APPEND)
		printf(", append");
	if (val & O_NONBLOCK)
		printf(", nonblocking");
	if (val & O_SYNC)
		printf(", synchronous writes");
}

int main(void) {
	
	int fd;
	mode_t mode = S_IRUSR | S_IWUSR;

	if ((fd = open("gov.no", O_WRONLY | O_APPEND |O_CREAT | O_TRUNC, mode)) < 0)
		err_sys("error call open");

	
	set_fl(fd, O_SYNC);
	exit(0);
}
