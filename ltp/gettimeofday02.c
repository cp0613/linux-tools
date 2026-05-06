// https://github.com/linux-test-project/ltp/blob/master/testcases/kernel/syscalls/gettimeofday/gettimeofday02.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

#define MAX_ALLOWED_DRIFT_US 1000000 // 1秒，防止系统暂停/休眠导致误报

int main(int argc, char *argv[])
{
	struct timeval tv1, tv2;
	long long diff_us;
	int i;
	int errors = 0;
	int TEST_ITERATIONS = 1000;

	if (argc == 2)
		TEST_ITERATIONS = atoi(argv[1]);

	printf("Testing gettimeofday() consistency (%d times)...\n", TEST_ITERATIONS);

	if (gettimeofday(&tv1, NULL) != 0) {
		perror("gettimeofday failed");
		return EXIT_FAILURE;
	}

	for (i = 0; i < TEST_ITERATIONS; i++) {
		if (gettimeofday(&tv2, NULL) != 0) {
			perror("gettimeofday failed in loop");
			return EXIT_FAILURE;
		}

		/* 计算时间差（微秒） */
		diff_us = (tv2.tv_sec - tv1.tv_sec) * 1000000LL + (tv2.tv_usec - tv1.tv_usec);

		/* 检查是否倒退（时间回跳） */
		if (diff_us < 0) {
			fprintf(stderr, "ERROR: Time went backwards! "
							"tv1={%ld,%ld}, tv2={%ld,%ld}, diff=%lld us\n",
					tv1.tv_sec, tv1.tv_usec,
					tv2.tv_sec, tv2.tv_usec, diff_us);
			errors++;
		}

		/* 检查是否跳变过大（可能因 NTP/手动设置，但短时间内不应发生） */
		if (diff_us > MAX_ALLOWED_DRIFT_US) {
			fprintf(stderr, "WARNING: Large time jump (%lld us) detected. "
							"This may be due to NTP or system sleep.\n",
					diff_us);
			/* 不计为错误，但提示 */
		}

		/* 更新 tv1 */
		tv1 = tv2;

		/* 微小延迟，避免 CPU 占满 */
		usleep(100); // 0.1ms
	}

	if (errors == 0) {
		printf("PASS: gettimeofday() behaved consistently over %d iterations.\n", TEST_ITERATIONS);
		return EXIT_SUCCESS;
	} else {
		printf("FAIL: %d time regression(s) detected!\n", errors);
		return EXIT_FAILURE;
	}
}
