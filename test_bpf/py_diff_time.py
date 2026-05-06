import re

def extract_data(file):
    data = {} # 创建一个空字典来保存提取的数据
    with open(file, 'r') as f:
        for line in f:
            parts = line.split() # 按空格分割行
            if parts: # 确保行非空
                # 使用正则表达式搜索以#开头的序号
                seq_match = re.search(r'#(\d+)', line)
                # 如果找到序号并且行中有足够的部分
                if seq_match and len(parts) >= 2:
                    try:
                        # 尝试将倒数第二列转化为整数
                        time = int(parts[-2]) # 倒数第二列时间
                    except ValueError:
                        # 如果转化失败（即倒数第二列不是数字），则忽略改行
                        continue
                    seq = int(seq_match.group(1)) # 序号

                    if "jited:1" in line:
                        data[seq] = {'time': time, 'jited':1}
                    else:
                        data[seq] = {'time': time, 'jited':0}
    return data

# 计算最大值、最小值和平均值等
def calc_statistics(ratios, lable):
    max_ratio = 0
    min_ratio = 0
    avg_ratio = 0
    count_lh095 = 0
    count_lh1 = 0
    count_gh5 = 0
    count_gh10 = 0

    if len(ratios) > 0:
        max_ratio = max(ratios)
        min_ratio = min(ratios)
        avg_ratio = sum(ratios) / len(ratios)

        for i in range(len(ratios)):
            if ratios[i] < 0.95:
                count_lh095 += 1
            if ratios[i] < 1:
                count_lh1 += 1
            if ratios[i] > 5:
                count_gh5 += 1
            if ratios[i] > 10:
                count_gh10 += 1

    print(f"{lable}")
    print(f"max: {max_ratio:.2f}")
    print(f"min: {min_ratio:.2f}")
    print(f"avg: {avg_ratio:.2f}")
    print(f"count_lh095: {count_lh095}")
    print(f"count_lh1: {count_lh1}")
    print(f"count_gh5: {count_gh5}")
    print(f"count_gh10: {count_gh10}")


# 提取两个文件中的数据
folder = 'qemu/'
data1 = extract_data(folder + 's32ilp32.txt.strip.sync')
data2 = extract_data(folder + 's64lp64.txt.strip')

# 初始化比值的列表
ratios_jited = []
ratios_jited_ne = []

# 计算两个时间的比值并输出
for seq in data1.keys():
    if seq in data2:
        time1 = data1[seq]['time']
        time2 = data2[seq]['time']
        jited1 = data1[seq]['jited']
        jited2 = data2[seq]['jited']
        # 确保时间2不为0以避免除以0的错误
        if time2 != 0:
            ratio = time1 / time2
            if jited1 == jited2: # 包含都被JIT或都没有JIT的情况，都是可以直接比的
                ratios_jited.append(ratio)
            else:
                ratios_jited_ne.append(ratio)
            print(f"#{seq}: {ratio} jited1:{jited1} jited2:{jited2}")
        else:
            print(f"#{seq}:time2 is zero, cannot compute ratio.")

calc_statistics(ratios_jited, '>>>JITed')
calc_statistics(ratios_jited_ne, '>>>NOT-JITed')
