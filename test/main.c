/* test suit by Deekseek */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main() {
    int16_t src[] = { 0x1234, 0x5678 }; // ����С����
    char target[4];
    memcpy(target, src, sizeof(src));

    // ��ӡtarget���ֽ����ݣ�С���������ӦΪ 34 12 78 56��
    for (int i = 0; i < 4; i++) {
        printf("target[%d] = 0x%02x\n", i, (unsigned char)target[i]);
    }
    return 0;
}