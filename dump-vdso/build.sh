gcc dump-vdso.c -o dump-vdso_host -static
riscv64-linux-gcc dump-vdso.c -o dump-vdso_rv64 -static
riscv32-linux-gcc dump-vdso.c -o dump-vdso_rv32 -static

# ./dump-vdso > vdso.so
# objdump -T vdso.so
