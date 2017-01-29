#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ 27

main () {
	char c;
	int shmid;
	key_t key = 5678; /* пусть ключ нашего сегмента памяти будет 5678 */
	char *shm, *s;
	/* Создаем сегмент */
	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		exit(1);
	}

	/* Присоединяем сегмент к адресному пространству нашего процесса */
	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		exit(1);
	}

	/* Положим в память английский алфавит */

	s = shm;
	for (c = 'a'; c <= 'z'; c++)
		*s++ = c;
	*s = NULL;

	while (*shm != '*')
		sleep(1);
	
	exit(0);
}
