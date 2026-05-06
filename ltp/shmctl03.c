// https://github.com/linux-test-project/ltp/blob/master/testcases/kernel/syscalls/ipc/shmctl/shmctl03.c

#define _GNU_SOURCE

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void verify_ipcinfo(void)
{
    int ret;
	struct shminfo info;

	ret = shmctl(0, IPC_INFO, (struct shmid_ds *)&info);
	if (ret < 0) {
		printf("TFAIL|TTERRNO shmctl(0, IPC_INFO, ...) returned %d\n", ret);
		return;
	}

	if (info.shmmin != 1)
		printf("TFAIL shmmin = %li, expected 1\n", info.shmmin);
	else
		printf("TPASS shmmin = 1\n");

	printf("check /proc/sys/kernel/shmmax == %lx\n", info.shmmax);
	printf("check /proc/sys/kernel/shmmni == %lx\n", info.shmmni);
	printf("check /proc/sys/kernel/shmall == %lx\n", info.shmall);
}

int main()
{
    verify_ipcinfo();

    return 0;
}