import re

# 定义需要处理的文件名列表
folder = 'qemu/'
filenames = [folder + 's32ilp32.txt', folder + 's64lp64.txt']

# 正则表达式用于匹配方括号及其内容
pattern = re.compile(r'\[.*?\]')

# 遍历文件名列表
for file_name in filenames:
    # 读取原始文件内容
    try:
        with open(file_name, 'r') as file:
            lines = file.readlines()
    except FileNotFoundError:
        print(f"The file {file_name} was not found.")
        continue
    except IOError as e:
        print(f"An I/O error occurred while processing {file_name}: {e}")
        continue

    # 处理每一行，删除方括号及其内容
    modified_lines = [pattern.sub('', line) for line in lines]

    # 将修改后的内容写回到文件中
    try:
        with open(file_name + '.strip', 'w') as file:
            file.writelines(modified_lines)
    except IOError as e:
        print(f"An I/O error occurred while writing to {file_name + '.strip'}: {e}")
