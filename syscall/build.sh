#gcc syscall_perf_getpid.c -o syscall_perf_getpid
riscv64-linux-gcc syscall_perf_getpid.c -o syscall_perf_getpid_rv64 -static
riscv32-linux-gcc syscall_perf_getpid.c -o syscall_perf_getpid_rv32 -static

#gcc syscall_mremap.c -o syscall_mremap
riscv64-linux-gcc syscall_mremap.c -o syscall_mremap_rv64 -static
riscv32-linux-gcc syscall_mremap.c -o syscall_mremap_rv32 -static