#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>


uint32_t left_rotate(uint32_t x, uint32_t n)
{
    return (x << n) | (x >> (32 - n));
}


int s[64] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5, 9, 14, 20,   5, 9, 14, 20,   5, 9, 14, 20,   5, 9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

static void init_md5_K(uint32_t K[64])
{
    for (int i = 0; i < 64; i++)
    {
        K[i] = (uint32_t)(fabs(sin(i + 1)) * 4294967296.0);
    }
}

char* md5(const char *message)
{
    static int md5_k_initialized = 0;
    static uint32_t md5_K[64];
    if (!md5_k_initialized)
    {
        init_md5_K(md5_K);
        md5_k_initialized = 1;
    }

    size_t initial_len = strlen(message);
    uint64_t bit_len = (uint64_t)initial_len * 8;

    size_t new_len = initial_len + 1;
    while (new_len % 64 != 56) {
        new_len++;
    }
    new_len += 8;

    unsigned char *msg = (unsigned char*) malloc(new_len);
    if (msg == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    memcpy(msg, message, initial_len);
    msg[initial_len] = 0x80;

    for (size_t i = initial_len + 1; i < new_len; i++) {
        msg[i] = 0;
    }

    memcpy(msg + new_len - 8, &bit_len, 8);

    uint32_t a0 = 0x67452301;
    uint32_t b0 = 0xefcdab89;
    uint32_t c0 = 0x98badcfe;
    uint32_t d0 = 0x10325476;

    for (size_t offset = 0; offset < new_len; offset += 64) {
        uint32_t M[16];
        int j;
        for (j = 0; j < 16; j++) {
            M[j] = (uint32_t) msg[offset + j*4] |
                   ((uint32_t) msg[offset + j*4 + 1] << 8) |
                   ((uint32_t) msg[offset + j*4 + 2] << 16) |
                   ((uint32_t) msg[offset + j*4 + 3] << 24);
        }

        uint32_t A = a0;
        uint32_t B = b0;
        uint32_t C = c0;
        uint32_t D = d0;

        for (j = 0; j < 64; j++) {
            uint32_t F = 0;
            int g = 0;
            if (j < 16) {
                F = (B & C) | ((~B) & D);
                g = j;
            } else if (j < 32) {
                F = (D & B) | ((~D) & C);
                g = (5 * j + 1) % 16;
            } else if (j < 48) {
                F = B ^ C ^ D;
                g = (3 * j + 5) % 16;
            } else {
                F = C ^ (B | (~D));
                g = (7 * j) % 16;
            }

            uint32_t temp = A + F + md5_K[j] + M[g];
            A = D;
            D = C;
            C = B;
            B = B + left_rotate(temp, s[j]);
        }

        a0 += A;
        b0 += B;
        c0 += C;
        d0 += D;
    }

    free(msg);

    unsigned char digest_bytes[16];
    memcpy(digest_bytes,      &a0, 4);
    memcpy(digest_bytes + 4,  &b0, 4);
    memcpy(digest_bytes + 8,  &c0, 4);
    memcpy(digest_bytes + 12, &d0, 4);

    char *digest = (char*) malloc(33);
    if (digest == NULL) {
        printf("Fucking malloc failed!\n");
        exit(1);
    }

    for (int i = 0; i < 16; i++) {
        sprintf(digest + i * 2, "%02x", digest_bytes[i]);
    }
    digest[32] = '\0';

    return digest;
}

#ifdef MD5_TEST

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <string>\n", argv[0]);
        return 1;
    }
    char *hash = md5(argv[1]);
    printf("%s\n", hash);
    free(hash);
    return 0;
}
#endif