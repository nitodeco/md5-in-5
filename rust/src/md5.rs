use std::convert::TryInto;

#[inline(always)]
fn left_rotate(x: u32, n: u32) -> u32 {
    x.rotate_left(n)
}

fn calc_md5_s() -> [u32; 64] {
    let mut s = [0u32; 64];
    let s1 = [7, 12, 17, 22];
    let s2 = [5, 9, 14, 20];
    let s3 = [4, 11, 16, 23];
    let s4 = [6, 10, 15, 21];
    for i in 0..16 {
        s[i] = s1[i % 4];
        s[i + 16] = s2[i % 4];
        s[i + 32] = s3[i % 4];
        s[i + 48] = s4[i % 4];
    }
    s
}

fn calc_md5_k() -> [u32; 64] {
    let mut k = [0u32; 64];
    let two_32 = 4294967296f64;
    for i in 0..64 {
        k[i] = ((two_32 * ((i as f64 + 1.0).sin().abs())).floor()) as u32;
    }
    k
}

pub fn md5(message: &str) -> String {
    let s = calc_md5_s();
    let k = calc_md5_k();

    let mut data = message.as_bytes().to_vec();
    let bit_len = (data.len() as u64) * 8;

    data.push(0x80);

    let padding = (56usize.wrapping_sub(data.len() % 64)) % 64;
    data.extend(vec![0u8; padding]);

    data.extend_from_slice(&bit_len.to_le_bytes());

    let mut a0: u32 = 0x67452301;
    let mut b0: u32 = 0xefcdab89;
    let mut c0: u32 = 0x98badcfe;
    let mut d0: u32 = 0x10325476;

    for chunk in data.chunks(64) {
        let mut m = [0u32; 16];
        for i in 0..16 {
            let j = i * 4;
            m[i] = u32::from_le_bytes(chunk[j..j + 4].try_into().unwrap());
        }

        let mut a = a0;
        let mut b = b0;
        let mut c = c0;
        let mut d = d0;

        for i in 0..64 {
            let (f, g) = if i < 16 {
                ((b & c) | ((!b) & d), i)
            } else if i < 32 {
                ((d & b) | ((!d) & c), (5 * i + 1) % 16)
            } else if i < 48 {
                (b ^ c ^ d, (3 * i + 5) % 16)
            } else {
                (c ^ (b | (!d)), (7 * i) % 16)
            };

            let temp = a
                .wrapping_add(f)
                .wrapping_add(k[i])
                .wrapping_add(m[g]);
            let temp = left_rotate(temp, s[i]);
            let new_b = b.wrapping_add(temp);

            a = d;
            d = c;
            c = b;
            b = new_b;
        }

        a0 = a0.wrapping_add(a);
        b0 = b0.wrapping_add(b);
        c0 = c0.wrapping_add(c);
        d0 = d0.wrapping_add(d);
    }

    let mut digest = String::with_capacity(32);
    for &word in &[a0, b0, c0, d0] {
        for byte in word.to_le_bytes().iter() {
            digest.push_str(&format!("{:02x}", byte));
        }
    }
    digest
}