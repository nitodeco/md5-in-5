import math

SHIFT_AMOUNTS = (
    [7, 12, 17, 22] * 4 + [5, 9, 14, 20] * 4 + [4, 11, 16, 23] * 4 + [6, 10, 15, 21] * 4
)

K = [int((2**32) * abs(math.sin(i + 1))) & 0xFFFFFFFF for i in range(64)]


def left_rotate(x: int, amount: int) -> int:
    x &= 0xFFFFFFFF
    return ((x << amount) | (x >> (32 - amount))) & 0xFFFFFFFF


def md5(message: str) -> str:
    data = message.encode("utf-8")
    original_length = (8 * len(data)) & 0xFFFFFFFFFFFFFFFF

    data += b"\x80"

    padding = (56 - (len(data) % 64)) % 64
    data += b"\x00" * padding

    data += original_length.to_bytes(8, byteorder="little")

    A = 0x67452301
    B = 0xEFCDAB89
    C = 0x98BADCFE
    D = 0x10325476

    for chunk_start in range(0, len(data), 64):
        chunk = data[chunk_start : chunk_start + 64]

        M = [
            int.from_bytes(chunk[i * 4 : (i + 1) * 4], byteorder="little")
            for i in range(16)
        ]

        a, b, c, d = A, B, C, D

        for i in range(64):
            if i < 16:
                f = (b & c) | ((~b & 0xFFFFFFFF) & d)
                g = i
            elif i < 32:
                f = (d & b) | ((~d & 0xFFFFFFFF) & c)
                g = (5 * i + 1) % 16
            elif i < 48:
                f = b ^ c ^ d
                g = (3 * i + 5) % 16
            else:
                f = c ^ (b | (~d & 0xFFFFFFFF))
                g = (7 * i) % 16

            f &= 0xFFFFFFFF
            temp = (a + f + K[i] + M[g]) & 0xFFFFFFFF
            temp = left_rotate(temp, SHIFT_AMOUNTS[i])

            new_b = (b + temp) & 0xFFFFFFFF
            a, b, c, d = d, new_b, b, c

        A = (A + a) & 0xFFFFFFFF
        B = (B + b) & 0xFFFFFFFF
        C = (C + c) & 0xFFFFFFFF
        D = (D + d) & 0xFFFFFFFF

    digest_bytes = (
        A.to_bytes(4, byteorder="little")
        + B.to_bytes(4, byteorder="little")
        + C.to_bytes(4, byteorder="little")
        + D.to_bytes(4, byteorder="little")
    )
    return "".join(f"{byte:02x}" for byte in digest_bytes)
