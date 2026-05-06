#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

static int test_mremap_basic(void)
{
    void *addr1, *addr2;
    size_t old_size = 4096;      // 1 page
    size_t new_size = 8192;      // 2 pages

    /* Step 1: 分配初始内存 */
    addr1 = mmap(NULL, old_size, PROT_READ | PROT_WRITE,
                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr1 == MAP_FAILED) {
        perror("mmap failed");
        return -1;
    }

    /* 写入测试数据 */
    memset(addr1, 0x55, old_size);

    /* Step 2: 扩展内存（不移动） */
    addr2 = mremap(addr1, old_size, new_size, MREMAP_MAYMOVE);
    if (addr2 == MAP_FAILED) {
        perror("mremap failed");
        munmap(addr1, old_size);
        return -1;
    }

    /* 验证原数据是否保留 */
    if (memcmp(addr2, "\x55", 1) != 0) {
        fprintf(stderr, "Data corruption after mremap!\n");
        munmap(addr2, new_size);
        return -1;
    }

    /* 写入新区域数据 */
    memset((char*)addr2 + old_size, 0xAA, new_size - old_size);

    /* 验证新区域 */
    if (memcmp((char*)addr2 + old_size, "\xAA", 1) != 0) {
        fprintf(stderr, "New region not writable!\n");
        munmap(addr2, new_size);
        return -1;
    }

    printf("Basic mremap test passed.\n");
    munmap(addr2, new_size);
    return 0;
}

static int test_mremap_shrink(void)
{
    void *addr1, *addr2;
    size_t old_size = 8192;
    size_t new_size = 4096;

    addr1 = mmap(NULL, old_size, PROT_READ | PROT_WRITE,
                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr1 == MAP_FAILED) {
        perror("mmap failed");
        return -1;
    }

    memset(addr1, 0x33, old_size);

    /* 缩小内存 */
    addr2 = mremap(addr1, old_size, new_size, 0); // 不允许移动
    if (addr2 == MAP_FAILED) {
        perror("mremap shrink failed");
        munmap(addr1, old_size);
        return -1;
    }

    /* 验证数据保留 */
    if (memcmp(addr2, "\x33", 1) != 0) {
        fprintf(stderr, "Shrink: data lost!\n");
        munmap(addr2, new_size);
        return -1;
    }

    printf("Shrink mremap test passed.\n");
    munmap(addr2, new_size);
    return 0;
}

static int test_mremap_move(void)
{
    void *addr1, *addr2, *addr3;
    size_t size = 4096;

    /* 分配两块相邻内存（制造碎片） */
    addr1 = mmap(NULL, size, PROT_READ | PROT_WRITE,
                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    addr2 = mmap(NULL, size, PROT_READ | PROT_WRITE,
                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (addr1 == MAP_FAILED || addr2 == MAP_FAILED) {
        perror("mmap failed");
        if (addr1 != MAP_FAILED) munmap(addr1, size);
        if (addr2 != MAP_FAILED) munmap(addr2, size);
        return -1;
    }

    /* 释放中间区域，制造空洞 */
    if (munmap((char*)addr1 + size, size) != 0) {
        perror("munmap hole failed");
        munmap(addr1, size);
        munmap(addr2, size);
        return -1;
    }

    /* 尝试扩展 addr1 跨越空洞（必须移动） */
    addr3 = mremap(addr1, size, size*2, MREMAP_MAYMOVE);
    if (addr3 == MAP_FAILED) {
        perror("mremap move failed");
        munmap(addr1, size);
        munmap(addr2, size);
        return -1;
    }

    /* 验证是否移动（地址应不同） */
    if (addr3 == addr1) {
        fprintf(stderr, "Expected move, but address unchanged!\n");
        munmap(addr3, size*2);
        munmap(addr2, size);
        return -1;
    }

    printf("Move mremap test passed.\n");
    munmap(addr3, size*2);
    munmap(addr2, size);
    return 0;
}

static int test_mremap_fixed(void)
{
#ifdef MREMAP_FIXED
    void *addr1, *addr2, *target;
    size_t size = 4096;

    addr1 = mmap(NULL, size, PROT_READ | PROT_WRITE,
                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr1 == MAP_FAILED) {
        perror("mmap failed");
        return -1;
    }

    /* 申请一个目标地址 */
    target = mmap(NULL, size*2, PROT_NONE,
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (target == MAP_FAILED) {
        perror("target mmap failed");
        munmap(addr1, size);
        return -1;
    }
    munmap(target, size*2); // 释放，留出空洞

    /* 强制重映射到指定地址 */
    addr2 = mremap(addr1, size, size, MREMAP_MAYMOVE | MREMAP_FIXED, target);
    if (addr2 == MAP_FAILED) {
        perror("mremap FIXED failed");
        munmap(addr1, size);
        return -1;
    }

    if (addr2 != target) {
        fprintf(stderr, "MREMAP_FIXED did not use target address!\n");
        munmap(addr2, size);
        return -1;
    }

    printf("MREMAP_FIXED test passed.\n");
    munmap(addr2, size);
    return 0;
#else
    printf("MREMAP_FIXED not supported, skip.\n");
    return 0;
#endif
}

int main(void)
{
    int ret = 0;

    printf("=== mremap() standalone test ===\n");

    ret |= test_mremap_basic();
    ret |= test_mremap_shrink();
    ret |= test_mremap_move();
    ret |= test_mremap_fixed();

    if (ret == 0) {
        printf("\nAll tests PASSED!\n");
        return EXIT_SUCCESS;
    } else {
        printf("\nSome tests FAILED!\n");
        return EXIT_FAILURE;
    }
}
