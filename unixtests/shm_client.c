#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ     27

main()
{
    int shmid;
    key_t key = 5678;
    char *shm, *s;


    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /* Присоединяем сегмент к адресному пространству процесса*/
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /* Читаем то что сервер записал в сегмент */
    for (s = shm; *s != NULL; s++)
        putchar(*s);
    putchar('\n');

    /* Ставим звёздочку*/
    *shm = '*';

    exit(0);
}
