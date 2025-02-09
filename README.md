# MD 5 in 5

From-scratch implementation of the MD5 algorithm in 5 different languages:

- Python
- TypeScript
- Go
- C
- Rust

## What is MD5?

MD5 (Message-Digest Algorithm 5) is a hashing algorithm. It takes an input of arbitrary size and produces a 128-bit / 16-byte hash. It works in 4 steps:

1. Padding: The input data is extended so its total length is almost a multiple of 512 bits. A `1` is added at the end, followed by `0`s, and the original length of the message is recorded in the last 64 bits

2. Chunking: The padded data is split into 512-bit chunks

3. Processing: Each chunk goes through 64 rounds of calculations using simple math operations like addition and bit shifts. Four values (A-D) are updated in each round

4. Finalizing: The values of A, B, C, and D are combined to form the 128-bit hash result

## Deprecation

MD5 should no longer be used in cryptographic applications due to its susceptibility to collision attacks, allowing malicious actors to craft different inputs resulting in identical hash values. More secure hash functions like SHA-256 have become dominant.
