import os
import random

def generate_random_file(file_name, size):
    with open(file_name, 'wb') as f:
        f.write(os.urandom(size))

# 生成一个大小为 2^4 到 2^5 的随机文件
generate_random_file('random_file_16_32.bin', random.randint(2**4, 2**5))

# 生成两个大小为 2^5 到 2^6 的随机文件
for i in range(2):
    generate_random_file(f'random_file_32_64_{i}.bin', random.randint(2**5, 2**6))

# 生成四个大小为 2^6 到 2^7 的随机文件
for i in range(4):
    generate_random_file(f'random_file_64_128_{i}.bin', random.randint(2**6, 2**7))

# 以此类推，一直到生成大小为 2^12 的随机文件
for power in range(7, 13):  # 从 2^7 到 2^12
    num_files = 2**(power-5)  # 文件数量为 2^(power-5)
    for i in range(num_files):
        file_size = random.randint(2**power, 2**(power+1))
        generate_random_file(f'random_file_{2**power}_{2**(power+1)}_{i}.bin', file_size)
        
for power in range(13, 18):  # 从 2^7 到 2^12
    num_files = 2**(power-10)  # 文件数量为 2^(power-5)
    for i in range(num_files):
        file_size = random.randint(2**power, 2**(power+1))
        generate_random_file(f'random_file_{2**power}_{2**(power+1)}_{i}.bin', file_size)
print("Random files have been generated.")