#include <stdio.h>
#include <stdint.h>

#include "address_translate.h"

uint16_t calculate_12bit_hash(uint64_t addr, uint64_t addr_mask) {
    
    uint8_t idx;
    uint8_t masked_addr_lsb; // Least Significant Bit, default is 6
    uint8_t masked_addr_msb; // Most Significant Bit,  default is 51
    uint16_t hash12 = 0;
    uint64_t masked_addr = addr & addr_mask;
    uint16_t group[6] = {0}; // 64bit val has max 6 loop if 12 bit in each loop
    uint16_t valid_length;
    uint16_t loop_time;
    uint16_t left_bit;

    /* Calculate the lowest and most significant bits based on addr_mask */
    masked_addr_lsb = LOWEST_NONZERO_POSITION(addr_mask);
    masked_addr_msb = HIGHEST_NONZERO_POSITION(addr_mask);

    /* Calculate the effective bit length in maskd_addr */
    valid_length = masked_addr_msb - masked_addr_lsb + 1;

    /**
     * @brief Calculate hash for every 12 bits, the number of times it needs
     * to be calculated, and the remaining extra bits are not calculated
     */
    loop_time = valid_length / 12;
    left_bit  = valid_length % 12;

    printf("LSB-MSB: %d-%d\n", masked_addr_lsb, masked_addr_msb);
    printf("valid_length: %d\n", valid_length);
    printf("loop_time: %d\n", loop_time);
    printf("left_bit: %d\n", left_bit);
    printf("addr: 0x%lx\n", addr);
    printf("masked_addr: 0x%lx\n", masked_addr);

    /* Starting from maskd_addr_lsb, put a group[idx] every 12 bits */
    for (idx = 0; idx < loop_time; idx++) {
        group[idx] = EXTRACT_BITS_64(masked_addr, masked_addr_lsb + 12 * idx, 12);
        printf("group[%d]: 0x%04x\n", idx, group[idx]);
    }

    /* Put the remaining bits into the last group */
    group[loop_time] = EXTRACT_BITS_64(masked_addr, masked_addr_lsb + loop_time * 12, left_bit);
    printf("group[%d]: 0x%04x\n", loop_time, group[loop_time]);

    for (idx = 0; idx < loop_time + 1; idx++) {
        hash12 ^= group[idx];
    }

    return hash12;

}



uint8_t calculate_non_power2hashing(uint64_t addr, uint8_t hnf_count_per_cluster, uint64_t addr_mask)
{
    Byte temp = {.byte = 0};
    uint8_t select = 0;
    uint16_t hash12 = 0;

    hash12 = calculate_12bit_hash(addr, addr_mask);

    temp.bit5 = BIT_N(hash12, 11) ^ BIT_N(hash12, 0);
    temp.bit4 = BIT_N(hash12, 10) ^ BIT_N(hash12, 1);
    temp.bit3 = BIT_N(hash12, 9) ^ BIT_N(hash12, 2);
    temp.bit2 = BIT_N(hash12, 8) ^ BIT_N(hash12, 3);
    temp.bit1 = BIT_N(hash12, 7) ^ BIT_N(hash12, 4);
    temp.bit0 = BIT_N(hash12, 6) ^ BIT_N(hash12, 5);

    select = (((temp.byte << 6) | EXTRACT_BITS_64(hash12, 0, 6)) * hnf_count_per_cluster) >> 12;

    return select;
}

void test_calculate_12bit_hash(void) {
    // Test cases
    uint64_t addr = 0x180080000000; // Example address 0x1800_8000_0000
    uint64_t addr1 = 0x3FFC003FFFF; // Example address 0x3FF_C003_FFFF
    uint64_t addr2 = 0xFFC003FFC0000; // Example address 0xF_FC00_3FFC_0000
    uint64_t addr_mask = 0xFFFFFFFFFFFC0;  // Extract 6 low bits, which means the granularity of the interlace is 64B

    printf("12bit_hash = 0x%x\n", calculate_12bit_hash(addr, addr_mask));
    printf("\n");
    printf("12bit_hash = 0x%x\n", calculate_12bit_hash(addr1, addr_mask));
    printf("\n");
    printf("12bit_hash = 0x%x\n", calculate_12bit_hash(addr2, addr_mask));
    printf("\n");
}

void test_calculate_non_power2hashing(){
    // Test cases
    uint64_t addr = 0x180080000000; // Example address 0x1800_8000_0000
    uint64_t addr1 = 0x3FFC003FFFF; // Example address 0x3FF_C003_FFFF
    uint64_t addr2 = 0xFFC003FFC0000; // Example address 0xF_FC00_3FFC_0000
    uint64_t addr_mask = 0xFFFFFFFFFFFC0;  // Extract 6 low bits, which means the granularity of the interlace is 64B
    uint8_t hnf_count_per_cluster = 7;

    printf("non_power2hashing = 0x%x\n", calculate_non_power2hashing(addr, hnf_count_per_cluster, addr_mask));
    printf("\n");
    printf("non_power2hashing = 0x%x\n", calculate_non_power2hashing(addr1, hnf_count_per_cluster, addr_mask));
    printf("\n");
    printf("non_power2hashing = 0x%x\n", calculate_non_power2hashing(addr2, hnf_count_per_cluster, addr_mask));
    printf("\n");
}

int main() {
    // Test cases
    // test_calculate_12bit_hash();
    test_calculate_non_power2hashing();
}
