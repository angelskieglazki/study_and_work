#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *src, *dst;
  int fin, fout;
  struct stat sb;

  fin = open("foo", O_RDONLY);
  fstat(fin, &sb);
  src = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fin, 0);

  fout = open("bar", O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
  ftruncate(fout, sb.st_size);
  dst = mmap(NULL, sb.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fout, 0);
  memcpy(dst, src, sb.st_size);
  msync(dst, sb.st_size, MS_SYNC);

  exit(0);
}
