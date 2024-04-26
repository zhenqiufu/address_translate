#ifndef __ADDRESS_TRANS_H
#define __ADDRESS_TRANS_H

#include <stdio.h>
#include <stdint.h>

// 宏定义计算64位变量的第n位，其中n为位偏移量
#define BIT_N(x, n) (((x) >> (n)) & 0x01)

// 宏定义从64位数据中截取指定大小的数据
#define EXTRACT_BITS_64(data, start, size) (((data) >> (start)) & ((1ULL << (size)) - 1))

// 宏定义计算64位变量中最低位非零值的位置
#define LOWEST_NONZERO_POSITION(x) ({ \
    int position = -1; \
    for (int i = 0; i < 64; ++i) { \
        if (BIT_N(x, i)) { \
            position = i; \
            break; \
        } \
    } \
    position; \
})

// 宏定义计算64位变量中最高位非零值的位置
#define HIGHEST_NONZERO_POSITION(x) ({ \
    int position = -1; \
    for (int i = 63; i >= 0; --i) { \
        if (BIT_N(x, i)) { \
            position = i; \
            break; \
        } \
    } \
    position; \
})

typedef union {
    struct {
        unsigned int bit0 : 1;
        unsigned int bit1 : 1;
        unsigned int bit2 : 1;
        unsigned int bit3 : 1;
        unsigned int bit4 : 1;
        unsigned int bit5 : 1;
        unsigned int bit6 : 1;
        unsigned int bit7 : 1;
    };
    uint8_t byte;
} Byte;


#define TARGET_GRP_2     2
#define TARGET_GRP_4     4
#define TARGET_GRP_8     8
#define TARGET_GRP_16    16
#define TARGET_GRP_32    32
#define TARGET_GRP_64    64
#define TARGET_GRP_128   128
#define TARGET_GRP_256   256

#endif