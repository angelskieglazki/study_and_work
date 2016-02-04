#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

void list_file( char* name) {
  struct stat sb;
  char *mod_time;

  char *file_type[] = {"?", "p", "c", "?", "d", "?", "b", "?", "-", "?",
                      "|", "?", "s"};

  if (stat(name , &sb) < 0) {
    perror(name);
    exit(2);
  }
  /* Тип фай ла (обо зна че ния как в ls) */
  printf("%s", file_type[(sb.st_mode >> 12) & 017]);
  /* Раз ре ше ния (как в ls) */
  printf("%c%c%c%c%c%c%c%c%c",
  (sb.st_mode & S_IRUSR) ? 'r' : '-',
  (sb.st_mode & S_IWUSR) ? 'w' : '-',
  (sb.st_mode & S_IXUSR) ? 'x' : '-',
  (sb.st_mode & S_IRGRP) ? 'r' : '-',
  (sb.st_mode & S_IWGRP) ? 'w' : '-',
  (sb.st_mode & S_IXGRP) ? 'x' : '-',
  (sb.st_mode & S_IROTH) ? 'r' : '-',
  (sb.st_mode & S_IWOTH) ? 'w' : '-',
  (sb.st_mode & S_IXOTH) ? 'x' : '-' );
  printf("%8ld", sb.st_size);
  mod_time = ctime(&sb.st_mtime);
  /* У стро ки ctime() в кон це \n, уда ля ем */
/*  mod_time[strlen(mod_time) - 1] = '\0';
  printf(" %s ", mod_time);
*/  printf("%s\n", name);
}


void main() {
  DIR *d;
  struct dirent *info; /* a directory entry */
  struct stat sb; /* the stat buffer*/
  long total = 0; /* total of file size*/

  d = opendir(".");

  while((info = readdir(d)) != NULL) {
    //stat(info->d_name, &sb);
    //total += sb.st_size;
    list_file(info->d_name);
  }

  closedir(d);
//  printf("All data size = %ld\n", total);
}
