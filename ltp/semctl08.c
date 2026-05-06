// https://github.com/linux-test-project/ltp/blob/master/testcases/kernel/syscalls/ipc/semctl/semctl08.c

#include <stdio.h>

#ifdef HAVE_SEMID64_DS_TIME_HIGH

void run(void)
{
	struct semid64_ds buf_ds = {
		.sem_otime_high = 0x0A0A,
		.sem_ctime_high = 0x0A0A,
	};
	int semid;
	union semun arg;
	key_t key;

	/* get an IPC resource key */
	key = GETIPCKEY();

	semid = semget(key, 1, SEM_RA | IPC_CREAT);
	if (semid == -1) {
		printf("TBROK|TERRNO couldn't create semaphore\n");
		return;
	}

	arg.buf = (struct semid_ds *)&buf_ds;
	if (semctl(semid, 0, IPC_STAT, arg) == -1) {
		printf("TFAIL|TTERRNO semctl() failed\n");
		return;
	}

	if (buf_ds.sem_otime_high || buf_ds.sem_ctime_high)
		printf("TFAIL time_high fields aren't cleared by the kernel\n");
	else
		printf("TPASS time_high fields cleared by the kernel\n");

	if (semctl(semid, 0, IPC_RMID, arg) == -1)
		printf("TINFO WARNING: semaphore deletion failed.\n");
}
#else
void run(void)
{
    printf("test requires struct semid64_ds to have the time_high fields\n");
}
#endif

int main()
{
    run();

    return 0;
}
