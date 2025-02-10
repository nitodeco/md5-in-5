package md5in5

import (
	"math"
)

var shiftAmounts = func() [64]uint32 {
	var s [64]uint32
	s1 := [4]uint32{7, 12, 17, 22}
	s2 := [4]uint32{5, 9, 14, 20}
	s3 := [4]uint32{4, 11, 16, 23}
	s4 := [4]uint32{6, 10, 15, 21}

	for i := 0; i < 16; i++ {
		s[i] = s1[i%4]
		s[i+16] = s2[i%4]
		s[i+32] = s3[i%4]
		s[i+48] = s4[i%4]
	}
	return s
}()

var k [64]uint32

func init() {
	for i := 0; i < 64; i++ {
		k[i] = uint32(math.Abs(math.Sin(float64(i+1))) * 4294967296)
	}
}

func leftRotate(x uint32, n uint32) uint32 {
	return (x << n) | (x >> (32 - n))
}

func MD5(message string) string {
	data := []byte(message)
	originalLen := uint64(len(data)) * 8
	data = append(data, 0x80)

	padLen := 0
	if rem := len(data) % 64; rem < 56 {
		padLen = 56 - rem
	} else {
		padLen = 64 - rem + 56
	}
	data = append(data, make([]byte, padLen)...)

	var lenBytes [8]byte
	for i := 0; i < 8; i++ {
		lenBytes[i] = byte((originalLen >> (8 * uint(i))) & 0xff)
	}
	data = append(data, lenBytes[:]...)

	a0 := uint32(0x67452301)
	b0 := uint32(0xefcdab89)
	c0 := uint32(0x98badcfe)
	d0 := uint32(0x10325476)

	for offset := 0; offset < len(data); offset += 64 {
		var M [16]uint32
		for i := 0; i < 16; i++ {
			j := offset + i*4
			M[i] = uint32(data[j]) | uint32(data[j+1])<<8 | uint32(data[j+2])<<16 | uint32(data[j+3])<<24
		}

		A := a0
		B := b0
		C := c0
		D := d0

		for i := 0; i < 64; i++ {
			var f uint32
			var g int
			switch {
			case i < 16:
				f = (B & C) | ((^B) & D)
				g = i
			case i < 32:
				f = (D & B) | ((^D) & C)
				g = (5*i + 1) % 16
			case i < 48:
				f = B ^ C ^ D
				g = (3*i + 5) % 16
			default:
				f = C ^ (B | (^D))
				g = (7 * i) % 16
			}

			temp := A + f + k[i] + M[g]
			temp = leftRotate(temp, shiftAmounts[i])
			temp = B + temp

			A, B, C, D = D, temp, B, C
		}

		a0 += A
		b0 += B
		c0 += C
		d0 += D
	}

	digest := make([]byte, 16)
	digest[0] = byte(a0 & 0xff)
	digest[1] = byte((a0 >> 8) & 0xff)
	digest[2] = byte((a0 >> 16) & 0xff)
	digest[3] = byte((a0 >> 24) & 0xff)

	digest[4] = byte(b0 & 0xff)
	digest[5] = byte((b0 >> 8) & 0xff)
	digest[6] = byte((b0 >> 16) & 0xff)
	digest[7] = byte((b0 >> 24) & 0xff)

	digest[8] = byte(c0 & 0xff)
	digest[9] = byte((c0 >> 8) & 0xff)
	digest[10] = byte((c0 >> 16) & 0xff)
	digest[11] = byte((c0 >> 24) & 0xff)

	digest[12] = byte(d0 & 0xff)
	digest[13] = byte((d0 >> 8) & 0xff)
	digest[14] = byte((d0 >> 16) & 0xff)
	digest[15] = byte((d0 >> 24) & 0xff)

	hexDigits := "0123456789abcdef"
	result := make([]byte, 32)
	for i, b := range digest {
		result[i*2] = hexDigits[b>>4]
		result[i*2+1] = hexDigits[b&0x0f]
	}
	return string(result)
}