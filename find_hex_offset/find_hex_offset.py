import sys
import binascii

def find_hex_offsets(file_path, hex_pattern):
    """
    查找文件中所有匹配给定十六进制模式的偏移地址。
    
    :param file_path: 要搜索的文件路径
    :param hex_pattern: 十六进制字符串（如 'DEADBEEF' 或 'DE AD BE EF'）
    :return: 包含所有匹配偏移的列表
    """
    try:
        # 清理输入的十六进制字符串（去除空格、转换为字节）
        hex_clean = hex_pattern.replace(' ', '').strip()
        if len(hex_clean) % 2 != 0:
            raise ValueError("十六进制字符串长度必须为偶数")
        pattern_bytes = binascii.unhexlify(hex_clean)
    except binascii.Error as e:
        print(f"无效的十六进制字符串: {e}")
        return []
    except Exception as e:
        print(f"错误: {e}")
        return []

    offsets = []

    try:
        with open(file_path, 'rb') as f:
            file_data = f.read()
    except FileNotFoundError:
        print(f"错误：文件 '{file_path}' 不存在")
        return []
    except PermissionError:
        print(f"错误：无法读取文件 '{file_path}'（权限不足）")
        return []
    except Exception as e:
        print(f"读取文件时发生错误: {e}")
        return []

    pattern_len = len(pattern_bytes)

    # 逐字节查找匹配
    for i in range(len(file_data) - pattern_len + 1):
        if file_data[i:i+pattern_len] == pattern_bytes:
            offsets.append(i)

    return offsets

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("用法: python find_hex.py <文件路径> <十六进制字符串>")
        print("示例: python find_hex.py test.bin DEADBEEF")
        print("      python find_hex.py test.bin 'DE AD BE EF'")
        sys.exit(1)

    file_path = sys.argv[1]
    hex_pattern = sys.argv[2]

    print(f"正在查找文件 '{file_path}' 中的十六进制序列 '{hex_pattern}' 的偏移...")

    offsets = find_hex_offsets(file_path, hex_pattern)

    if offsets:
        print(f"找到 {len(offsets)} 个匹配项：")
        for offset in offsets:
            print(f"偏移: 0x{offset:08X}")
    else:
        print("未找到匹配的十六进制序列。")
