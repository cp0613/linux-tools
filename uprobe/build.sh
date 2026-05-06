riscv32-linux-gcc test.c -o test_rv32 -g
riscv64-linux-gcc test.c -o test_rv64 -g

#echo 'p:foo ./test:0x10400' >> /sys/kernel/debug/tracing/uprobe_events
#cat /sys/kernel/debug/tracing/uprobe_events
#echo 1 >/sys/kernel/debug/tracing/events/uprobes/enable
#echo 1 >/sys/kernel/debug/tracing/events/uprobes/foo/enable
