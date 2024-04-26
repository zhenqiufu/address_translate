#include <stdio.h>
#include <stdint.h>

#include "address_translate.h"

// 计算log2的函数
uint64_t log2x(uint64_t x) {
    if (x == 0) {
        return -1; // 如果输入为0，则返回-1表示无效输入
    }

    uint64_t result = 0;
    // 反复除以2，直到x为1
    while (x > 1) {
        x >>= 1; // 右移一位，相当于除以2
        result++;
    }

    return result;
}

/**
 * @brief Using the power of 2 hash algorithm, calculate which target
 * node (e.g. HNF) index the address should be placed in.
 * 
 * @param addr             Address to be calculated
 * @param target_group_num Number of target HNFs
 * @param addr_mask        Interleave granularity
 * @return uint8_t         Target node index (e.g. HNF)
 */
uint8_t powerof2hashing(uint64_t addr, uint16_t target_group_num, uint64_t addr_mask) {

    uint64_t masked_addr = addr & addr_mask;

    uint8_t idx;
    uint8_t masked_addr_lsb; // Least Significant Bit, default is 6
    uint8_t masked_addr_msb; // Most Significant Bit,  default is 51
    uint8_t bit_num_in_select;

    Byte select = {.byte = 0};
    masked_addr_lsb = LOWEST_NONZERO_POSITION(addr_mask);
    masked_addr_msb = HIGHEST_NONZERO_POSITION(addr_mask);

    switch (target_group_num) {
        case TARGET_GRP_2:
            for (idx = masked_addr_lsb; idx <= masked_addr_msb; idx++) {
                select.bit0 ^= BIT_N(masked_addr, idx);
                printf("idx: %02d val: %ld select.bit0: %d \n", idx, BIT_N(masked_addr, idx), select.bit0);
            }
            break;

        case TARGET_GRP_4:
            bit_num_in_select = (uint8_t)log2x(TARGET_GRP_4);
            for (idx = masked_addr_lsb; idx <= masked_addr_msb; idx++) {
                idx % bit_num_in_select == 0 ? (select.bit0 ^= BIT_N(masked_addr, idx)) :
                                               (select.bit1 ^= BIT_N(masked_addr, idx));

                idx % bit_num_in_select == 0 ? (printf("idx: %02d val: %ld select.bit0: %d     ", idx, BIT_N(masked_addr, idx), select.bit0)) :
                                 (printf("idx: %02d val: %ld select.bit1: %d   \n", idx, BIT_N(masked_addr, idx), select.bit1));
            }
            printf("\n");
            break;

        case TARGET_GRP_8:
            bit_num_in_select = (uint8_t)log2x(TARGET_GRP_8);
            for (idx = masked_addr_lsb; idx <= masked_addr_msb; idx++) {
                idx % bit_num_in_select == 0 ? (select.bit0 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 1 ? (select.bit1 ^= BIT_N(masked_addr, idx)) :
                                 (select.bit2 ^= BIT_N(masked_addr, idx));

                idx % bit_num_in_select == 0 ? (printf("idx: %02d val: %ld select.bit0: %d     ", idx, BIT_N(masked_addr, idx), select.bit0)) :
                idx % bit_num_in_select == 1 ? (printf("idx: %02d val: %ld select.bit1: %d     ", idx, BIT_N(masked_addr, idx), select.bit1)) :
                                 (printf("idx: %02d val: %ld select.bit2: %d   \n", idx, BIT_N(masked_addr, idx), select.bit2));

            }
            printf("\n");
            break;

        case TARGET_GRP_16:
            bit_num_in_select = (uint8_t)log2x(TARGET_GRP_16);
            for (idx = masked_addr_lsb; idx <= masked_addr_msb; idx++) {
                idx % bit_num_in_select == 2 ? (select.bit0 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 3 ? (select.bit1 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 0 ? (select.bit2 ^= BIT_N(masked_addr, idx)) :
                                 (select.bit3 ^= BIT_N(masked_addr, idx));

                idx % bit_num_in_select == 2 ? (printf("idx: %02d val: %ld select.bit0: %d     ", idx, BIT_N(masked_addr, idx), select.bit0)) :
                idx % bit_num_in_select == 3 ? (printf("idx: %02d val: %ld select.bit1: %d     ", idx, BIT_N(masked_addr, idx), select.bit1)) :
                idx % bit_num_in_select == 0 ? (printf("idx: %02d val: %ld select.bit2: %d     ", idx, BIT_N(masked_addr, idx), select.bit2)) :
                                 (printf("idx: %02d val: %ld select.bit3: %d   \n", idx, BIT_N(masked_addr, idx), select.bit3));

            }
            printf("\n");
            break;

        case TARGET_GRP_32:
            bit_num_in_select = (uint8_t)log2x(TARGET_GRP_32);
            for (idx = masked_addr_lsb; idx <= masked_addr_msb; idx++) {
                idx % bit_num_in_select == 1 ? (select.bit0 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 2 ? (select.bit1 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 3 ? (select.bit2 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 4 ? (select.bit3 ^= BIT_N(masked_addr, idx)) :
                                 (select.bit4 ^= BIT_N(masked_addr, idx));

                idx % bit_num_in_select == 1 ? (printf("idx: %02d val: %ld select.bit0: %d     ", idx, BIT_N(masked_addr, idx), select.bit0)) :
                idx % bit_num_in_select == 2 ? (printf("idx: %02d val: %ld select.bit1: %d     ", idx, BIT_N(masked_addr, idx), select.bit1)) :
                idx % bit_num_in_select == 3 ? (printf("idx: %02d val: %ld select.bit2: %d     ", idx, BIT_N(masked_addr, idx), select.bit2)) :
                idx % bit_num_in_select == 4 ? (printf("idx: %02d val: %ld select.bit3: %d     ", idx, BIT_N(masked_addr, idx), select.bit3)) :
                                 (printf("idx: %02d val: %ld select.bit4: %d   \n", idx, BIT_N(masked_addr, idx), select.bit4));

            }
            printf("\n");
            break;
        
        case TARGET_GRP_64:
            bit_num_in_select = (uint8_t)log2x(TARGET_GRP_64);
            for (idx = masked_addr_lsb; idx <= masked_addr_msb; idx++) {
                idx % bit_num_in_select == 0 ? (select.bit0 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 1 ? (select.bit1 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 2 ? (select.bit2 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 3 ? (select.bit3 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 4 ? (select.bit4 ^= BIT_N(masked_addr, idx)) :
                                       (select.bit5 ^= BIT_N(masked_addr, idx));

                idx % bit_num_in_select == 0 ? (printf("idx: %02d val: %ld select.bit0: %d     ", idx, BIT_N(masked_addr, idx), select.bit0)) :
                idx % bit_num_in_select == 1 ? (printf("idx: %02d val: %ld select.bit1: %d     ", idx, BIT_N(masked_addr, idx), select.bit1)) :
                idx % bit_num_in_select == 2 ? (printf("idx: %02d val: %ld select.bit2: %d     ", idx, BIT_N(masked_addr, idx), select.bit2)) :
                idx % bit_num_in_select == 3 ? (printf("idx: %02d val: %ld select.bit3: %d     ", idx, BIT_N(masked_addr, idx), select.bit3)) :
                idx % bit_num_in_select == 4 ? (printf("idx: %02d val: %ld select.bit4: %d     ", idx, BIT_N(masked_addr, idx), select.bit4)) :
                                       (printf("idx: %02d val: %ld select.bit5: %d   \n", idx, BIT_N(masked_addr, idx), select.bit5));

            }
            printf("\n");
            break;
        
        case TARGET_GRP_128:
            bit_num_in_select = (uint8_t)log2x(TARGET_GRP_128);
            for (idx = masked_addr_lsb; idx <= masked_addr_msb; idx++) {
                idx % bit_num_in_select == 6 ? (select.bit0 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 0 ? (select.bit1 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 1 ? (select.bit2 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 2 ? (select.bit3 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 3 ? (select.bit4 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 4 ? (select.bit5 ^= BIT_N(masked_addr, idx)) :
                                               (select.bit6 ^= BIT_N(masked_addr, idx));

                idx % bit_num_in_select == 6 ? (printf("idx: %02d val: %ld select.bit0: %d     ", idx, BIT_N(masked_addr, idx), select.bit0)) :
                idx % bit_num_in_select == 0 ? (printf("idx: %02d val: %ld select.bit1: %d     ", idx, BIT_N(masked_addr, idx), select.bit1)) :
                idx % bit_num_in_select == 1 ? (printf("idx: %02d val: %ld select.bit2: %d     ", idx, BIT_N(masked_addr, idx), select.bit2)) :
                idx % bit_num_in_select == 2 ? (printf("idx: %02d val: %ld select.bit3: %d     ", idx, BIT_N(masked_addr, idx), select.bit3)) :
                idx % bit_num_in_select == 3 ? (printf("idx: %02d val: %ld select.bit4: %d     ", idx, BIT_N(masked_addr, idx), select.bit4)) :
                idx % bit_num_in_select == 4 ? (printf("idx: %02d val: %ld select.bit5: %d     ", idx, BIT_N(masked_addr, idx), select.bit5)) :
                                               (printf("idx: %02d val: %ld select.bit6: %d   \n", idx, BIT_N(masked_addr, idx), select.bit6));

            }
            printf("\n");
            break;
        
        case TARGET_GRP_256:
            bit_num_in_select = (uint8_t)log2x(TARGET_GRP_256);
            for (idx = masked_addr_lsb; idx <= masked_addr_msb; idx++) {
                idx % bit_num_in_select == 6 ? (select.bit0 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 7 ? (select.bit1 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 0 ? (select.bit2 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 1 ? (select.bit3 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 2 ? (select.bit4 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 3 ? (select.bit5 ^= BIT_N(masked_addr, idx)) :
                idx % bit_num_in_select == 4 ? (select.bit6 ^= BIT_N(masked_addr, idx)) :
                                               (select.bit7 ^= BIT_N(masked_addr, idx));

                idx % bit_num_in_select == 6 ? (printf("idx: %02d val: %ld select.bit0: %d     ", idx, BIT_N(masked_addr, idx), select.bit0)) :
                idx % bit_num_in_select == 7 ? (printf("idx: %02d val: %ld select.bit1: %d     ", idx, BIT_N(masked_addr, idx), select.bit1)) :
                idx % bit_num_in_select == 0 ? (printf("idx: %02d val: %ld select.bit2: %d     ", idx, BIT_N(masked_addr, idx), select.bit2)) :
                idx % bit_num_in_select == 1 ? (printf("idx: %02d val: %ld select.bit3: %d     ", idx, BIT_N(masked_addr, idx), select.bit3)) :
                idx % bit_num_in_select == 2 ? (printf("idx: %02d val: %ld select.bit4: %d     ", idx, BIT_N(masked_addr, idx), select.bit4)) :
                idx % bit_num_in_select == 3 ? (printf("idx: %02d val: %ld select.bit5: %d     ", idx, BIT_N(masked_addr, idx), select.bit5)) :
                idx % bit_num_in_select == 4 ? (printf("idx: %02d val: %ld select.bit6: %d     ", idx, BIT_N(masked_addr, idx), select.bit6)) :
                                               (printf("idx: %02d val: %ld select.bit7: %d   \n", idx, BIT_N(masked_addr, idx), select.bit7));

            }
            printf("\n");
            break;

        default:
            // Handle invalid target_group_num vals
            // For simplicity, just return 0
            break;
    }

    return select.byte;
}


int main() {
    // Test cases
    uint64_t addr = 0x180080000000; // Example address 0x1800_8000_0000
    uint64_t addr_mask = 0xFFFFFFFFFFFC0;  // Extract 6 low bits, which means the granularity of the interlace is 64B
    uint16_t target_group_num;

    // Test target_group_num = 2
    target_group_num = 2;
    printf("Test target_group_num = 2\n");
    printf("target_group_num = %d: %d\n", target_group_num, powerof2hashing(addr, target_group_num, addr_mask));
    printf("\n");

    // Test target_group_num = 4
    target_group_num = 4;
    printf("Test target_group_num = 4\n");
    printf("target_group_num = %d: %d\n", target_group_num, powerof2hashing(addr, target_group_num, addr_mask));
    printf("\n");

    // Test target_group_num = 8
    target_group_num = 8;
    printf("Test target_group_num = 8\n");
    printf("target_group_num = %d: %d\n", target_group_num, powerof2hashing(addr, target_group_num, addr_mask));
    printf("\n");

    // Test target_group_num = 16
    target_group_num = 16;
    printf("Test target_group_num = 16\n");
    printf("target_group_num = %d: %d\n", target_group_num, powerof2hashing(addr, target_group_num, addr_mask));
    printf("\n");

    // Test target_group_num = 32
    target_group_num = 32;
    printf("Test target_group_num = 32\n");
    printf("target_group_num = %d: %d\n", target_group_num, powerof2hashing(addr, target_group_num, addr_mask));
    printf("\n");

    // Test target_group_num = 64
    target_group_num = 64;
    printf("Test target_group_num = 64\n");
    printf("target_group_num = %d: %d\n", target_group_num, powerof2hashing(addr, target_group_num, addr_mask));
    printf("\n");

    // Test target_group_num = 128
    target_group_num = 128;
    printf("Test target_group_num = 128\n");
    printf("target_group_num = %d: %d\n", target_group_num, powerof2hashing(addr, target_group_num, addr_mask));
    printf("\n");

    // Test target_group_num = 256
    target_group_num = 256;
    printf("Test target_group_num = 256\n");
    printf("target_group_num = %d: %d\n", target_group_num, powerof2hashing(addr, target_group_num, addr_mask));
    printf("\n");

    return 0;
}
