#gcc sysinfo03.c -o sysinfo03 -lm
riscv64-linux-gcc sysinfo03.c -o sysinfo03_rv64 -lm -static
riscv32-linux-gcc sysinfo03.c -o sysinfo03_rv32 -lm -static

#gcc semctl08.c -o semctl08
riscv64-linux-gcc semctl08.c -o semctl08_rv64 -static
riscv32-linux-gcc semctl08.c -o semctl08_rv32 -static

#gcc shmctl03.c -o shmctl03
riscv64-linux-gcc shmctl03.c -o shmctl03_rv64 -static
riscv32-linux-gcc shmctl03.c -o shmctl03_rv32 -static

#gcc gettimeofday02.c -o gettimeofday02
riscv64-linux-gcc gettimeofday02.c -o gettimeofday02_rv64 -static
riscv32-linux-gcc gettimeofday02.c -o gettimeofday02_rv32 -static