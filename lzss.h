/*============================================================================*/
/*

		simple LZSS encoder / decoder
		written by Sebastian Steinhauer <s.steinhauer@yahoo.de>

*/
/*============================================================================*/
#ifndef __included__lzss_h__
#define __included__lzss_h__

#include <stddef.h>

typedef unsigned char lzss_byte; /* definition of a byte...adjust when needed */

size_t lzss_compress_bound(size_t len);

size_t lzss_compress(const lzss_byte *src, size_t src_len, lzss_byte *dst, size_t dst_len);

size_t lzss_decompress(const lzss_byte *src, size_t src_len, lzss_byte *dst, size_t dst_len);

#endif /* __included__lzss_h__ */
/*============================================================================*/
/*
	This is free and unencumbered software released into the public domain.

	Anyone is free to copy, modify, publish, use, compile, sell, or
	distribute this software, either in source code form or as a compiled
	binary, for any purpose, commercial or non-commercial, and by any
	means.

	In jurisdictions that recognize copyright laws, the author or authors
	of this software dedicate any and all copyright interest in the
	software to the public domain. We make this dedication for the benefit
	of the public at large and to the detriment of our heirs and
	successors. We intend this dedication to be an overt act of
	relinquishment in perpetuity of all present and future rights to this
	software under copyright law.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.

	For more information, please refer to <http://unlicense.org/>
*/
/*============================================================================*/
