#ifndef MUTATION
#define MUTATION
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
//
// Created by zzzccc on 3/23/24.
//
#define BLOCK_SIZE 4

//随机变异 (Random Mutations): 对输入数据的随机位进行翻转、设置、重置或随机替换。
void random_mutation(uint8_t *data, size_t size) {
    // 计算需要执行多少次变化
    size_t num_mutations = size / 2;

    for (size_t i = 0; i < num_mutations; ++i) {
        // 随机选择一个位置
        size_t index = rand() % size;

        // 随机选择一个操作
        int mutation_type = rand() % 4;

        // 执行随机操作
        switch (mutation_type) {
            case 0: // 翻转一个随机位
                data[index] ^= (1 << (rand() % 8));
                break;
            case 1: // 设置一个随机位
                data[index] |= (1 << (rand() % 8));
                break;
            case 2: // 重置一个随机位
                data[index] &= ~(1 << (rand() % 8));
                break;
            case 3: // 随机替换整个字节
                data[index] = (uint8_t)rand();
                break;
        }
    }
}

//Bit-flipping: 逐位翻转输入数据中的每个比特。
void bit_flipping(uint8_t *data, size_t size) {
    // 遍历数据中的每个比特
    for (size_t i = 0; i < size; ++i) {
        for (int j = 0; j < 8; ++j) {
            // 翻转当前位
            data[i] ^= (1 << j);
        }
    }
}

//Byte-flipping: 类似于 bit-flipping，但是以字节为单位。
void byte_flipping(uint8_t *data, size_t size) {
    // 遍历数据中的每个字节
    for (size_t i = 0; i < size; ++i) {
        // 翻转当前字节
        data[i] = ~data[i];
    }
}



void magic_numbers(uint8_t *data, size_t size, int *MAGIC_NUMBERS, size_t MAGIC_NUMBERS_COUNT) {
    if (size < sizeof(int)) return; // 确保至少有足够空间存放一个整数

    // 计算需要执行多少次变化
    size_t num_mutations = size / (8 * sizeof(int)); // 使用 8 倍的 int 大小来计算次数

    for (size_t i = 0; i < num_mutations; ++i) {
        // 随机选择一个位置，确保位置允许完整的 int 被写入
        size_t index = (rand() % (size - sizeof(int) + 1));

        // 随机选择一个Magic Number
        int magic = MAGIC_NUMBERS[rand() % MAGIC_NUMBERS_COUNT];

        // 替换选定位置的整数值
        memcpy(data + index, &magic, sizeof(int));
    }
}

void insert_delete_mutation(uint8_t **data_ptr, u32 *size_ptr) {
    uint8_t *data = *data_ptr;
    size_t size = *size_ptr;
    size_t insert_length;
    size_t position;


    if (size < 100) {
        // 如果长度小于100，百分百插入一段10-100长度的数据
        insert_length = (rand() % 91) + 10; // 10到100之间的随机长度
        data = realloc(data, size + insert_length);
        if (!data) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        position = rand() % (size + 1); // 插入的位置
        memmove(data + position + insert_length, data + position, size - position);
        for (size_t i = position; i < position + insert_length; i++) {
            data[i] = (uint8_t)rand(); // 插入随机数据
        }
        *size_ptr = size + insert_length;
    } else if (size >= 100 && size < 500) {
        // 如果长度大于等于100且小于500，有三分之二的概率插入
        if ((rand() % 3)  < 2) {
            insert_length = (rand() % 91) + 10; // 10到100之间的随机长度
            data = realloc(data, size + insert_length);
            if (!data) {
                perror("Failed to allocate memory");
                exit(EXIT_FAILURE);
            }
            position = rand() % (size + 1); // 插入的位置
            memmove(data + position + insert_length, data + position, size - position);
            for (size_t i = position; i < position + insert_length; i++) {
                data[i] = (uint8_t)rand(); // 插入随机数据
            }
            *size_ptr = size + insert_length;
        }else{
            insert_length = (rand() % 91) + 10; // 10到100之间的随机长度
            if (insert_length > size) {
                insert_length = size; // 确保不会删除超过缓冲区的数据
            }
            position = rand() % (size - insert_length + 1); // 删除的位置
            memmove(data + position, data + position + insert_length, size - position - insert_length+1);
            data = realloc(data, size - insert_length);
            if (!data) {
                perror("Failed to allocate memory");
                exit(EXIT_FAILURE);
            }
            *size_ptr = size - insert_length;
        }
    } else if (size >= 500) {
        // 如果长度大于等于500，有50%的概率进行操作
        if (rand() % 2) {
            // 50%概率插入
            insert_length = (rand() % 91) + 10; // 10到100之间的随机长度
            data = realloc(data, size + insert_length);
            if (!data) {
                perror("Failed to allocate memory");
                exit(EXIT_FAILURE);
            }
            position = rand() % (size + 1); // 插入的位置
            memmove(data + position + insert_length, data + position, size - position);
            for (size_t i = position; i < position + insert_length; i++) {
                data[i] = (uint8_t)rand(); // 插入随机数据
            }
            *size_ptr = size + insert_length;
        } else {
            // 50%概率删除
            insert_length = (rand() % 91) + 10; // 10到100之间的随机长度
            if (insert_length > size) {
                insert_length = size; // 确保不会删除超过缓冲区的数据
            }
            position = rand() % (size - insert_length + 1); // 删除的位置
            memmove(data + position, data + position + insert_length, size - position - insert_length+1);
            data = realloc(data, size - insert_length);
            if (!data) {
                perror("Failed to allocate memory");
                exit(EXIT_FAILURE);
            }
            *size_ptr = size - insert_length;
        }
    }

    *data_ptr = data; // 更新数据指针
}

//Block Mutations: 删除、复制、粘贴或替换数据块。
void block_mutations(uint8_t *data, size_t size) {
    if (size < BLOCK_SIZE) return; // 确保有足够的数据进行块操作

    size_t block_start = rand() % (size - BLOCK_SIZE + 1);

    // 随机选择一个操作
    int mutation_type = rand() % 3;

    switch (mutation_type) {
        case 0: // 删除一个数据块
            memmove(data + block_start, data + block_start + BLOCK_SIZE, size - block_start - BLOCK_SIZE);
            break;
        case 1: // 复制一个数据块到随机位置
        {
            size_t new_location = rand() % (size - BLOCK_SIZE + 1);
            memcpy(data + new_location, data + block_start, BLOCK_SIZE);
        }
            break;
        case 2: // 替换一个数据块
            for (size_t i = 0; i < BLOCK_SIZE; ++i) {
                data[block_start + i] = (uint8_t)rand();
            }
            break;
    }
}

//Byte-wise Arithmetic: 对输入数据中的字节进行小的算术操作，比如加一或减一
void byte_wise_arithmetic(uint8_t *data, size_t size) {
    // 随机选择一个位置
    size_t index = rand() % size;

    // 随机选择加一或减一操作
    if (rand() % 2) {
        data[index] += 1; // 加一
    } else {
        data[index] -= 1; // 减一
    }
}

//插入数据
void insertion(uint8_t **data, size_t *size, const uint8_t *insert_data, size_t insert_size) {
    size_t position = rand() % (*size + 1); // 在数据的任意位置插入

    // 重新分配内存以增加insert_size个字节
    *data = realloc(*data, *size + insert_size);
    if (!*data) {
        exit(1); // 如果内存分配失败，则退出
    }

    // 移动现有数据以腾出空间
    memmove(*data + position + insert_size, *data + position, *size - position);

    // 插入新数据
    memcpy(*data + position, insert_data, insert_size);
    *size += insert_size; // 更新数据大小
}

//删除
void deletion(uint8_t **data, size_t *size, size_t delete_size) {
    if (*size <= delete_size) {
        return; // 如果删除大小大于或等于数据大小，则不进行删除
    }

    size_t position = rand() % (*size - delete_size); // 随机选择删除位置

    // 移动数据来覆盖待删除的部分
    memmove(*data + position, *data + position + delete_size, *size - position - delete_size);

    // 调整内存大小
    *data = realloc(*data, *size - delete_size);
    if (!*data) {
        exit(1); // 如果内存分配失败，则退出
    }

    *size -= delete_size; // 更新数据大小
}

//替换
void replacement(uint8_t **data, size_t *size, const uint8_t *replace_data, size_t replace_size, size_t replace_position, size_t replace_length) {
    if (replace_position + replace_length > *size) {
        return; // 如果替换位置加上长度超出原始数据，不进行替换
    }

    // 计算新的数据大小
    size_t new_size = *size - replace_length + replace_size;

    // 重新分配内存
    *data = realloc(*data, new_size);
    if (!*data) {
        exit(1); // 如果内存分配失败，则退出
    }

    // 移动后面的数据来腾出或填补空间
    memmove(*data + replace_position + replace_size, *data + replace_position + replace_length, *size - replace_position - replace_length);

    // 插入新数据
    memcpy(*data + replace_position, replace_data, replace_size);

    // 更新数据大小
    *size = new_size;
}

//使用已知的有效结构（如文件头、特定格式的数据块）替换或插入。
void known_structs_mutation(uint8_t **data, size_t *size, const uint8_t *known_struct, size_t struct_size) {
    // 假设known_struct是有效的结构，如文件头
    size_t position = rand() % (*size + 1);
    *data = realloc(*data, *size + struct_size);
    if (*data == NULL) {
        exit(1);
    }
    memmove(*data + position + struct_size, *data + position, *size - position);
    memcpy(*data + position, known_struct, struct_size);
    *size += struct_size;
}
//使用预定义的词典或字符串列表替换或插入数据中的部分。
void dictionary_based_mutation(uint8_t **data, size_t *size, const uint8_t **dictionary, size_t dict_size) {
    size_t word_index = rand() % dict_size;
    const uint8_t *word = dictionary[word_index];
    size_t word_length = strlen((const char *)word);

    size_t position = rand() % (*size + 1);
    *data = realloc(*data, *size + word_length);
    if (*data == NULL) {
        exit(1);
    }
    memmove(*data + position + word_length, *data + position, *size - position);
    memcpy(*data + position, word, word_length);
    *size += word_length;
}

//  根据输入数据的文法结构生成或修改测试用例 这里假设我们有一个简单的文法来生成数字序列
void grammar_based_mutation(uint8_t **data, size_t *size) {
    const char *grammar = "1234567890";
    size_t length = strlen(grammar);

    size_t position = rand() % (*size + 1);
    *data = realloc(*data, *size + length);
    if (*data == NULL) {
        exit(1);
    }
    memmove(*data + position + length, *data + position, *size - position);
    memcpy(*data + position, grammar, length);
    *size += length;
}

//使用特定模式或序列替换数据的一部分。
void pattern_based_mutation(uint8_t **data, size_t *size, const uint8_t *pattern, size_t pattern_size) {
    // 假设pattern是我们想要插入的特定模式，比如"ABCD"
    size_t position = rand() % (*size + 1);
    *data = realloc(*data, *size + pattern_size);
    if (*data == NULL) {
        exit(1);
    }
    memmove(*data + position + pattern_size, *data + position, *size - position);
    memcpy(*data + position, pattern, pattern_size);
    *size += pattern_size;
}
#endif
