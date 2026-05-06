import re
import sys
import subprocess

def get_function_name(address, vmlinux_path):
    """
    使用 addr2line 解析地址对应的函数名
    """
    try:
        # 调用 addr2line 命令
        cmd = ["riscv64-unknown-linux-gnu-addr2line", "-e", vmlinux_path, "-f", address]
        result = subprocess.run(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            check=True
        )
        # 输出格式: "func_name\nfile:line"
        output = result.stdout.strip().split('\n')
        return output[0] if output else "??"
    except (subprocess.CalledProcessError, FileNotFoundError, IndexError):
        return "??"

def main(input_file, vmlinux_path, output_file):
    """
    主函数：匹配行 -> 提取地址 -> 解析函数名 -> 写入输出
    """
    # 正则表达式匹配目标行（支持 0x 前缀的地址）
    pattern = re.compile(r'^Cpu\s+(\d+)\s+PC\(VA\)\s+(0x[0-9a-fA-F]+)')

    with open(input_file, 'r', encoding='utf-8') as fin, \
         open(output_file, 'w', encoding='utf-8') as fout:

        for line in fin:
            match = pattern.match(line.strip())
            if match:
                cpu, addr = match.groups()
                func_name = get_function_name(addr, vmlinux_path)
                # 输出格式: 2273692 Cpu 0 PC(VA) 0xffffffff80a65788 function_name
                fout.write(f"Cpu {cpu} PC(VA) {addr} {func_name}\n")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python3 script.py <input_file> <vmlinux> <output_file>")
        sys.exit(1)

    input_path = sys.argv[1]
    vmlinux_path = sys.argv[2]
    output_path = sys.argv[3]

    main(input_path, vmlinux_path, output_path)
