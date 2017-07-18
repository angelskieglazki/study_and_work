/*#include <unistd.h>*/
/*int main(int arc, char *argv[])*/
/*{*/
/*char str[] = "Hello,  world!\n";*/
/*write(1,  str,  sizeof(str) - 1);*/
/*exit(0);*/
/*}*/

.data /*сегмент данных*/

hello_str:
        .string "Hello,  world!\n"

        .set hello_str_length, . - hello_str - 1

.text /*сегмент кода*/

.globl main /*main видно за пределами этого файла*/
.type main, @function /*main - функция*/

main:
        movl  $4, %eax /*помещаем номер системного вызова write = 4 в eax*/
        movl  $1, %ebx /*номер файлового деск. stdout = 1 - первый параметр*/
        movl  $hello_str, %ecx /*указатель на строку - второй параметр*/
        movl  $hello_str_length, %edx /*длина - третий парамерт*/
        int  $0x80

        movl  $1, %eax
        movl  $0, %ebx
        int  $0x80

        .size  main, .-main