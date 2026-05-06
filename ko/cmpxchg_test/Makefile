# 指定交叉编译器的前缀
CROSS_COMPILE ?= riscv64-unknown-linux-gnu-

# 指定目标系统的内核源代码路径
KERNEL_DIR ?= linux-6.6

# 模块对象文件
obj-m := cmpxchg_test.o

# 默认目标编译模块
all:
	$(MAKE) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=$(PWD) modules

# 清理编译生成的文件
clean:
	$(MAKE) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=$(PWD) clean
