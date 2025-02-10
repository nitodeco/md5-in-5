#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cstdint>
#include <cstring>

static inline uint32_t leftRotate(uint32_t x, uint32_t n) {
    return (x << n) | (x >> (32 - n));
}

class MD5 {
public:
    static std::string compute(const std::string &input) {
        uint32_t a0 = 0x67452301;
        uint32_t b0 = 0xefcdab89;
        uint32_t c0 = 0x98badcfe;
        uint32_t d0 = 0x10325476;

        size_t originalLen = input.size();
        uint64_t bitLen = originalLen * 8;
        size_t newLen = originalLen + 1;
        while (newLen % 64 != 56) {
            newLen++;
        }
        newLen += 8;

        std::vector<unsigned char> msg(newLen, 0);
        std::memcpy(msg.data(), input.data(), originalLen);
        msg[originalLen] = 0x80;

        std::memcpy(msg.data() + newLen - 8, &bitLen, 8);

        uint32_t K[64];
        for (int i = 0; i < 64; ++i) {
            K[i] = static_cast<uint32_t>(std::fabs(std::sin(i + 1)) * 4294967296.0);
        }

        int s[64] = {
             7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
             5, 9, 14, 20,   5, 9, 14, 20,   5, 9, 14, 20,   5, 9, 14, 20,
             4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
             6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
        };

        size_t totalChunks = msg.size() / 64;
        for (size_t chunk = 0; chunk < totalChunks; ++chunk) {
            uint32_t M[16] = {0};
            size_t offset = chunk * 64;
            for (int i = 0; i < 16; ++i) {
                M[i] = (uint32_t)msg[offset + i * 4] |
                       ((uint32_t)msg[offset + i * 4 + 1] << 8) |
                       ((uint32_t)msg[offset + i * 4 + 2] << 16) |
                       ((uint32_t)msg[offset + i * 4 + 3] << 24);
            }

            uint32_t A = a0;
            uint32_t B = b0;
            uint32_t C = c0;
            uint32_t D = d0;

            for (int i = 0; i < 64; ++i) {
                uint32_t F = 0;
                int g = 0;
                if (i < 16) {
                    F = (B & C) | ((~B) & D);
                    g = i;
                } else if (i < 32) {
                    F = (D & B) | ((~D) & C);
                    g = (5 * i + 1) % 16;
                } else if (i < 48) {
                    F = B ^ C ^ D;
                    g = (3 * i + 5) % 16;
                } else {
                    F = C ^ (B | (~D));
                    g = (7 * i) % 16;
                }
                uint32_t temp = A + F + K[i] + M[g];
                A = D;
                D = C;
                C = B;
                B = B + leftRotate(temp, s[i]);
            }

            a0 += A;
            b0 += B;
            c0 += C;
            d0 += D;
        }

        unsigned char digestBytes[16];
        std::memcpy(digestBytes,      &a0, 4);
        std::memcpy(digestBytes + 4,  &b0, 4);
        std::memcpy(digestBytes + 8,  &c0, 4);
        std::memcpy(digestBytes + 12, &d0, 4);

        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (int i = 0; i < 16; ++i) {
            oss << std::setw(2) << static_cast<int>(digestBytes[i]);
        }
        return oss.str();
    }
};

#ifdef MD5_TEST
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <string>\n";
        return 1;
    }
    std::string input = argv[1];
    std::string hash = MD5::compute(input);
    std::cout << hash << std::endl;
    return 0;
}
#endif