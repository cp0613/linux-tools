#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static unsigned long long sum = 0;

int main(int argc, char *argv[])
{
	int loop1 = 3000000, loop2 = 10000;
	if (argc == 2)
		loop1 = atoi(argv[1]);
	else if (argc == 3)
	{
		loop1 = atoi(argv[1]);
		loop2 = atoi(argv[2]);
	}
	else
	{
		printf("usage:\n\t %s <loop1> <loop2>\n", argv[0]);
		return 0;
	}

	while (1)
	{
		printf("big...\n");
		for (int i = 1; i <= loop1; i++)
		{
			sum += loop1;
			sum *= loop1;
			sum /= loop1;
		}

		printf("LITTLE...\n");
		for (int i = 1; i <= loop2; i++)
		{
			sum += loop1;
			sum *= loop1;
			sum /= loop1;
			usleep(1000);
		}
	}

	return 0;
}
