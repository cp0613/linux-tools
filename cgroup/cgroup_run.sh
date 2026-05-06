#!/bin/bash

# 检查是否提供了命令参数
if [ "$#" -lt 1 ]; then
    echo "使用方法: $0 <要运行的命令> [命令参数]"
    exit 1
fi

# 指定cgroup的名称
CGROUP_NAME="my_cgroup"
CGROUP_PATH="/sys/fs/cgroup/$CGROUP_NAME"

# 创建cgroup目录
cgcreate -g cpu:$CGROUP_NAME

# 把当前进程添加到cgroup中
echo $$ > $CGROUP_PATH/cgroup.procs

# 设置CPU限制
echo 95 > $CGROUP_PATH/cpuset.cpus

echo 2G > $CGROUP_PATH/memory.max

# 运行指定的命令
cgexec -g cpu:$CGROUP_NAME "$@"

# 清理cgroup (可选)
cgdelete -g cpu:$CGROUP_NAME

exit 0
