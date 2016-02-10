/* processing command line keys and args */
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  
  int c;
  int aflag = 0;
  int bflag = 0;
  int numoption = 0;
  char txtoption[200] = "default";
  opterr = 0;

  while ( (c = getopts(argc, argv, "abn:t:")) != EOF) {
    switch (c) {
      case 'a' : aflag = 1; break;
      case 'b' : bflag = 1; break;
      case 'n' : numoption = atoi(optarg); break;
      case 't' : strcpy(txtoption, optarg); break;
      case '?' : printf(stderr, "wrong option: -%c\n", optopt);
    }
  }

  if (aflag) printf("Parametr a is set\n");
  if (bflag) printf("Parametr b is set\n");
  printf("Num value of option %d\n", numoption);
  printf("Text value of option %s\n", txtoption);
  argv +=optind; 
  argc -= optind;

  while(argc--){
  printf("arg: %s\n", *argv++);
  }
  return 0;
}
