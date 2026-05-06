# 定义文件列表
folder = 'qemu/'
filenames = [folder + 's32ilp32.txt', folder + 's64lp64.txt']

# 遍历文件列表
for filename in filenames:
    print("filename:", filename)
    try:
        # 使用 with 语句打开文件，保证文件使用后能正确关闭
        with open(filename, 'r') as file:
            # 逐行读取文件内容
            for line in file:
                # 检查该行是否不包含 "jited:1" 关键字
                if "jited:1" not in line:
                    # 输出不包含关键字的行
                    print("	", line.strip())  # strip() 移除行尾的换行符
    except FileNotFoundError:
        print(f"File {filename} not found.")
    except IOError:
        print(f"Error reading from {filename}.")
