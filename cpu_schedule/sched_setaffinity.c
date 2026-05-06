#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

int main(int argc, char *argv[])
{
	int cpu_x = 0;
	if (argc == 2)
		cpu_x = atoi(argv[1]);
	else
	{
		printf("usage:\n\t %s <cpu>\n", argv[0]);
		return 0;
	}

	cpu_set_t mask;
	int status;

	// 清空CPU集以准备设置亲和性
	CPU_ZERO(&mask);

	// 假设我们想要运行在cpu_x上
	CPU_SET(cpu_x, &mask);

	// 设置当前进程的亲和性
	status = sched_setaffinity(0, sizeof(mask), &mask);
	if (status != 0)
	{
		perror("sched_setaffinity");
		return 1;
	}

	// 在这里执行你的主要工作
	while (1)
	{
		// 你的程序逻辑
	}

	return 0;
}
