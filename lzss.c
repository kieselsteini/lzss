/*============================================================================*/
#include "lzss.h"

/*============================================================================*/
#define POS_BITS 12	/* bits for encoding the position */
#define LEN_BITS (16 - POS_BITS) /* bits for encoding the length */
#define POS_SIZE (1 << POS_BITS) /* length of sliding window */
#define LEN_SIZE (1 << LEN_BITS) /* length of matching string */
#define LEN_MIN 3 /* minimum matching string length */

/*============================================================================*/
size_t lzss_compress_bound(size_t len) {
	return (len * 9) / 8 + 1;
}

/*============================================================================*/
size_t lzss_compress(const lzss_byte *src, size_t src_len, lzss_byte *dst, size_t dst_len) {
	const lzss_byte *ptr, *base = src;
	lzss_byte flags, *flags_ptr;
	int i, j, k, pos, len, mlen;
	size_t dst_written = 0;

	while (src_len > 0 && dst_written < dst_len) { /* compress everything */
		flags = 0; flags_ptr = dst++; ++dst_written; /* reserve byte for flags */
		for (k = 0; k < 8; ++k) { /* encode next 8 tokens */
			if ((src_len < 1) || (dst_written >= dst_len)) break; /* stop encoding when there's no input/output */
			pos = len = 0; /* reset position and length */
			mlen = LEN_SIZE + LEN_MIN - 1; /* determine max search length */
			if (mlen > src_len) mlen = src_len; /* cap at max input length */
			/* this is fucking slow O(POS_SIZE * LEN_SIZE) complexity :( */
			for (i = 0; i < POS_SIZE; ++i) { /* search in whole window */
				ptr = src - i - 1; /* find the position of already encoded stuff */
				if (ptr < base) break; /* don't search when window is not big enough */
				for (j = 0; j < mlen && j <= i && ptr[j] == src[j]; ++j); /* try to match the whole length */
				if (j > len) { pos = i; len = j; } /* save best match so far */
			}
			if (len < LEN_MIN) { /* encode single byte */
				flags |= (1 << k); /* set bit in flags byte */
				len = 1; /* just encode a single byte from input */
				if (dst_written < dst_len) { *dst++ = *src; ++dst_written; } /* copy byte to output */
			} else { /* encode reference */
				i = (pos << LEN_BITS) | (len - LEN_MIN); /* create 16-bit value */
				if (dst_written < dst_len) { *dst++ = i >> 8;  ++dst_written; } /* write hi-byte */
				if (dst_written < dst_len) { *dst++ = i & 255; ++dst_written; } /* write lo-byte */
			}
			src += len; src_len -= len; /* advance the input */
		}
		*flags_ptr = flags; /* write flags byte to output */
	}
	return dst_written;
}

/*============================================================================*/
size_t lzss_decompress(const lzss_byte *src, size_t src_len, lzss_byte *dst, size_t dst_len) {
	const lzss_byte *ptr, *base = dst;
	lzss_byte flags;
	int i, tmp, pos, len;
	size_t dst_written = 0;

	while (src_len > 0 && dst_written < dst_len) { /* decompress everything */
		flags = *src++; --src_len; /* read next flag byte */
		for (i = 0; i < 8; ++i) { /* decode next 8 tokens */
			if (dst_written >= dst_len) break; /* stop if output is full */
			if (flags & 1) { /* decode a single byte */
				if (src_len < 1) break; /* check for input */
				ptr = src++; --src_len; /* set the pointer to the next byte */
				len = 1; /* the length for copying is 1 */
			} else { /* decode a reference */
				if (src_len < 2) break; /* check for input */
				tmp = src[0] << 8 | src[1]; src_len -= 2; src += 2; /* read next 2 bytes */
				pos = tmp >> LEN_BITS; /* decode the position */
				len = (tmp % LEN_SIZE) + LEN_MIN; /* decode the length */
				ptr = dst - 1 - pos; /* compute the pointer position */
				if (ptr < base) break; /* stop if pointer is invalid */
			}
			flags >>= 1; /* use next bit in next iteration */
			for (; len > 0 && dst_written < dst_len; --len, ++dst_written)
				*dst++ = *ptr++; /* copy the bytes */
		}
	}
	return dst_written;
}

/*============================================================================*/
