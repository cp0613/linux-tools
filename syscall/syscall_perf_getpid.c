#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    struct timeval start, end;
    unsigned int i, iterations = 1000000; // 需要执行的系统调用次数

    if (argc == 2)
        iterations = atoi(argv[1]);

    gettimeofday(&start, NULL);

    for (i = 0; i < iterations; i++)
    {
        getpid(); // 要测试性能的系统调用

        //struct timespec ts;
        //clock_gettime(CLOCK_MONOTONIC, &ts);
    }

    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;

    printf("Time taken for %d iterations: %.3f seconds.\n", iterations, elapsed);
    printf("Average time per system call: %.3f microseconds.\n", (elapsed / iterations) * 1e6);
    return 0;
}
