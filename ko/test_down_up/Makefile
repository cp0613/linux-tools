# 指定交叉编译器的前缀
CROSS_COMPILE_5.10 ?= riscv64-unknown-linux-gnu-
CROSS_COMPILE_6.6 ?= riscv64-unknown-linux-gnu-

# 指定目标系统的内核源代码路径
KERNEL_DIR_5.10 ?= linux-5.10
KERNEL_DIR_6.6 ?= linux-6.6

# 目标模块名称
MODNAME := test_down_up
# 模块对象文件
obj-m := $(MODNAME).o

# 默认目标编译模块
all: 5.10 6.6

5.10:
	$(MAKE) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE_5.10) -C $(KERNEL_DIR_5.10) M=$(PWD) modules
	mv $(MODNAME).ko $(MODNAME)_$@.ko

6.6:
	$(MAKE) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE_6.6) -C $(KERNEL_DIR_6.6) M=$(PWD) modules
	mv $(MODNAME).ko $(MODNAME)_$@.ko

# 清理编译生成的文件
clean:
	$(MAKE) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE_5.10) -C $(KERNEL_DIR_5.10) M=$(PWD) clean
	$(MAKE) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE_6.6) -C $(KERNEL_DIR_6.6) M=$(PWD) clean
