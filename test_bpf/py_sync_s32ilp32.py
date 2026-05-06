import re

def edit_file(input_filename, output_filename, delete_keyword, end_keyword):
    # 开始读取文件
    with open(input_filename, 'r') as infile:
        lines = infile.readlines()
    
    with open(output_filename, 'w') as outfile:
        decrement_active = False  # 控制是否需要减小数字
        for line in lines:
            # 如果行包含删除关键字，则丢弃该行，并开始后续行的数字减一操作
            if delete_keyword in line:
                decrement_active = True
                continue
            # 如果遇到终止关键字，则写入当前行，并停止后续行的数字减一操作
            if end_keyword in line:
                outfile.write(line)
                decrement_active = False
                continue
            # 如果存在需要减小的数字，并且行中包含"#", 则减小数字
            if decrement_active and "#" in line:
                # 使用正则表达式来找到 # 后的第一个整数
                new_line = re.sub(r'(#[ ]*)(\d+)', lambda match: match.group(1) + str(int(match.group(2))-1), line)
                outfile.write(new_line)
            else:
                # 写入当前行，无论是否经过处理
                outfile.write(line)

folder = 'qemu/'
edit_file(folder + 's32ilp32.txt.strip', folder + 's32ilp32.txt.strip.sync', 'INT: 32-bit context pointer word order and zero-extension', 'Summary')
