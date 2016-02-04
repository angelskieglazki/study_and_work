#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>

void main() {
  DIR *d;
  struct dirent *info; /* a directory entry */
  struct stat sb; /* the stat buffer*/
  long total = 0; /* total of file size*/

  d = opendir(".");

  while((info = readdir(d)) != NULL) {
    stat(info->d_name, &sb);
    total += sb.st_size;
  }

  closedir(d);
  printf("All data size = %ld\n", total);
}
