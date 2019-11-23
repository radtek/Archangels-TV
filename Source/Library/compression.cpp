// COMPRESSION.CPP : Compression Related Interface Definition.
//
// This is a part of the SatView(TM) spacecraft operating system.
// Copyright� 1992-2016 by BINARY SPACE, Switzerland.
// All rights reserved.
//
// This source code contains the definition of the compression related
// interface.
//
/**********************************************************************
* Modifications:                                                     *
*====================================================================*
* Date       | Release | Comments                                    *
*------------|---------|---------------------------------------------*
* 2011/07/22 | 1.00.00 | Initial creation. [AW]                      *
**********************************************************************/

#include "SatView.h"

#include "compression.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* minilzo.h -- mini subset of the LZO real-time data compression library

This file is part of the LZO real-time data compression library.

Copyright (C) 1996-2011 Markus Oberhumer
All Rights Reserved.

The LZO library is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

The LZO library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with the LZO library; see the file COPYING.
If not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

Markus F.X.J. Oberhumer
<markus@oberhumer.com>
http://www.oberhumer.com/opensource/lzo/
*/

#define __LZO_IN_MINILZO 1

#ifndef __LZO_CONF_H
#define __LZO_CONF_H 1

#if (LZO_CC_BORLANDC && LZO_ARCH_I086)
#  pragma option -h
#endif

#if (LZO_CC_MSC && (_MSC_VER >= 1000))
#  pragma warning(disable: 4127 4701)
#endif
#if (LZO_CC_MSC && (_MSC_VER >= 1300))
#  pragma warning(disable: 4820 4995)
#  pragma warning(disable: 4514 4710 4711)
#endif

#if (LZO_CC_SUNPROC)
#if !defined(__cplusplus)
#  pragma error_messages(off,E_END_OF_LOOP_CODE_NOT_REACHED)
#  pragma error_messages(off,E_LOOP_NOT_ENTERED_AT_TOP)
#  pragma error_messages(off,E_STATEMENT_NOT_REACHED)
#endif
#endif

#if (__LZO_MMODEL_HUGE) && !(LZO_HAVE_MM_HUGE_PTR)
#  error "this should not happen - check defines for __huge"
#endif

#if (LZO_ARCH_I086)
#  define ACC_MM_AHSHIFT        LZO_MM_AHSHIFT
#  define ACC_PTR_FP_OFF(x)     (((const unsigned __far*)&(x))[0])
#  define ACC_PTR_FP_SEG(x)     (((const unsigned __far*)&(x))[1])
#  define ACC_PTR_MK_FP(s,o)    ((void __far*)(((unsigned long)(s)<<16)+(unsigned)(o)))
#endif

#if !defined(lzo_uintptr_t)
#  if defined(__LZO_MMODEL_HUGE)
#    define lzo_uintptr_t       unsigned long
#  elif 1 && defined(LZO_OS_OS400) && (LZO_SIZEOF_VOID_P == 16)
#    define __LZO_UINTPTR_T_IS_POINTER 1
typedef char*              lzo_uintptr_t;
#    define lzo_uintptr_t       lzo_uintptr_t
#  elif (LZO_SIZEOF_SIZE_T == LZO_SIZEOF_VOID_P)
#    define lzo_uintptr_t       size_t
#  elif (LZO_SIZEOF_LONG == LZO_SIZEOF_VOID_P)
#    define lzo_uintptr_t       unsigned long
#  elif (LZO_SIZEOF_INT == LZO_SIZEOF_VOID_P)
#    define lzo_uintptr_t       unsigned int
#  elif (LZO_SIZEOF_LONG_LONG == LZO_SIZEOF_VOID_P)
#    define lzo_uintptr_t       unsigned long long
#  else
#    define lzo_uintptr_t       size_t
#  endif
#endif
LZO_COMPILE_TIME_ASSERT_HEADER(sizeof(lzo_uintptr_t) >= sizeof(lzo_voidp))

#if 1 && !defined(LZO_CFG_FREESTANDING)
#if 1 && !defined(HAVE_STRING_H)
#define HAVE_STRING_H 1
#endif
#if 1 && !defined(HAVE_MEMCMP)
#define HAVE_MEMCMP 1
#endif
#if 1 && !defined(HAVE_MEMCPY)
#define HAVE_MEMCPY 1
#endif
#if 1 && !defined(HAVE_MEMMOVE)
#define HAVE_MEMMOVE 1
#endif
#if 1 && !defined(HAVE_MEMSET)
#define HAVE_MEMSET 1
#endif
#endif

#if 1 && defined(HAVE_STRING_H)
#include <string.h>
#endif

#if (LZO_CFG_FREESTANDING)
#  undef HAVE_MEMCMP
#  undef HAVE_MEMCPY
#  undef HAVE_MEMMOVE
#  undef HAVE_MEMSET
#endif

#if !(HAVE_MEMCMP)
#  undef memcmp
#  define memcmp(a,b,c)         lzo_memcmp(a,b,c)
#elif !(__LZO_MMODEL_HUGE)
#  undef lzo_memcmp
#  define lzo_memcmp(a,b,c)     memcmp(a,b,c)
#endif
#if !(HAVE_MEMCPY)
#  undef memcpy
#  define memcpy(a,b,c)         lzo_memcpy(a,b,c)
#elif !(__LZO_MMODEL_HUGE)
#  undef lzo_memcpy
#  define lzo_memcpy(a,b,c)     memcpy(a,b,c)
#endif
#if !(HAVE_MEMMOVE)
#  undef memmove
#  define memmove(a,b,c)        lzo_memmove(a,b,c)
#elif !(__LZO_MMODEL_HUGE)
#  undef lzo_memmove
#  define lzo_memmove(a,b,c)    memmove(a,b,c)
#endif
#if !(HAVE_MEMSET)
#  undef memset
#  define memset(a,b,c)         lzo_memset(a,b,c)
#elif !(__LZO_MMODEL_HUGE)
#  undef lzo_memset
#  define lzo_memset(a,b,c)     memset(a,b,c)
#endif

#undef NDEBUG
#if (LZO_CFG_FREESTANDING)
#  undef LZO_DEBUG
#  define NDEBUG 1
#  undef assert
#  define assert(e) ((void)0)
#else
#  if !defined(LZO_DEBUG)
#    define NDEBUG 1
#  endif
#  include <assert.h>
#endif

#if 0 && defined(__BOUNDS_CHECKING_ON)
#  include <unchecked.h>
#else
#  define BOUNDS_CHECKING_OFF_DURING(stmt)      stmt
#  define BOUNDS_CHECKING_OFF_IN_EXPR(expr)     (expr)
#endif

#if !defined(__lzo_inline)
#  define __lzo_inline              /*empty*/
#endif
#if !defined(__lzo_forceinline)
#  define __lzo_forceinline         /*empty*/
#endif
#if !defined(__lzo_noinline)
#  define __lzo_noinline            /*empty*/
#endif

#if (LZO_CFG_PGO)
#  undef __acc_likely
#  undef __acc_unlikely
#  undef __lzo_likely
#  undef __lzo_unlikely
#  define __acc_likely(e)       (e)
#  define __acc_unlikely(e)     (e)
#  define __lzo_likely(e)       (e)
#  define __lzo_unlikely(e)     (e)
#endif

#if 1
#  define LZO_BYTE(x)       ((unsigned char) (x))
#else
#  define LZO_BYTE(x)       ((unsigned char) ((x) & 0xff))
#endif

#define LZO_MAX(a,b)        ((a) >= (b) ? (a) : (b))
#define LZO_MIN(a,b)        ((a) <= (b) ? (a) : (b))
#define LZO_MAX3(a,b,c)     ((a) >= (b) ? LZO_MAX(a,c) : LZO_MAX(b,c))
#define LZO_MIN3(a,b,c)     ((a) <= (b) ? LZO_MIN(a,c) : LZO_MIN(b,c))

#define lzo_sizeof(type)    ((lzo_uint) (sizeof(type)))

#define LZO_HIGH(array)     ((lzo_uint) (sizeof(array)/sizeof(*(array))))

#define LZO_SIZE(bits)      (1u << (bits))
#define LZO_MASK(bits)      (LZO_SIZE(bits) - 1)

#define LZO_LSIZE(bits)     (1ul << (bits))
#define LZO_LMASK(bits)     (LZO_LSIZE(bits) - 1)

#define LZO_USIZE(bits)     ((lzo_uint) 1 << (bits))
#define LZO_UMASK(bits)     (LZO_USIZE(bits) - 1)

#if !defined(DMUL)
#if 0

#  define DMUL(a,b) ((lzo_xint) ((lzo_uint32)(a) * (lzo_uint32)(b)))
#else
#  define DMUL(a,b) ((lzo_xint) ((a) * (b)))
#endif
#endif

#if 1 && (LZO_ARCH_AMD64 || LZO_ARCH_I386 || LZO_ARCH_POWERPC)
#  if (LZO_SIZEOF_SHORT == 2)
#    define LZO_UNALIGNED_OK_2 1
#  endif
#  if (LZO_SIZEOF_INT == 4)
#    define LZO_UNALIGNED_OK_4 1
#  endif
#endif
#if 1 && (LZO_ARCH_AMD64)
#  if defined(LZO_UINT64_MAX)
#    define LZO_UNALIGNED_OK_8 1
#  endif
#endif
#if (LZO_CFG_NO_UNALIGNED)
#  undef LZO_UNALIGNED_OK_2
#  undef LZO_UNALIGNED_OK_4
#  undef LZO_UNALIGNED_OK_8
#endif

#undef UA_GET16
#undef UA_SET16
#undef UA_COPY16
#undef UA_GET32
#undef UA_SET32
#undef UA_COPY32
#undef UA_GET64
#undef UA_SET64
#undef UA_COPY64
#if defined(LZO_UNALIGNED_OK_2)
LZO_COMPILE_TIME_ASSERT_HEADER(sizeof(unsigned short) == 2)
#  if 1 && defined(ACC_UA_COPY16)
#    define UA_GET16        ACC_UA_GET16
#    define UA_SET16        ACC_UA_SET16
#    define UA_COPY16       ACC_UA_COPY16
#  else
#    define UA_GET16(p)     (* (__lzo_ua_volatile const lzo_ushortp) (__lzo_ua_volatile const lzo_voidp) (p))
#    define UA_SET16(p,v)   ((* (__lzo_ua_volatile lzo_ushortp) (__lzo_ua_volatile lzo_voidp) (p)) = (unsigned short) (v))
#    define UA_COPY16(d,s)  UA_SET16(d, UA_GET16(s))
#  endif
#endif
#if defined(LZO_UNALIGNED_OK_4) || defined(LZO_ALIGNED_OK_4)
LZO_COMPILE_TIME_ASSERT_HEADER(sizeof(lzo_uint32) == 4)
#  if 1 && defined(ACC_UA_COPY32)
#    define UA_GET32        ACC_UA_GET32
#    define UA_SET32        ACC_UA_SET32
#    define UA_COPY32       ACC_UA_COPY32
#  else
#    define UA_GET32(p)     (* (__lzo_ua_volatile const lzo_uint32p) (__lzo_ua_volatile const lzo_voidp) (p))
#    define UA_SET32(p,v)   ((* (__lzo_ua_volatile lzo_uint32p) (__lzo_ua_volatile lzo_voidp) (p)) = (lzo_uint32) (v))
#    define UA_COPY32(d,s)  UA_SET32(d, UA_GET32(s))
#  endif
#endif
#if defined(LZO_UNALIGNED_OK_8)
LZO_COMPILE_TIME_ASSERT_HEADER(sizeof(lzo_uint64) == 8)
#  if 1 && defined(ACC_UA_COPY64)
#    define UA_GET64        ACC_UA_GET64
#    define UA_SET64        ACC_UA_SET64
#    define UA_COPY64       ACC_UA_COPY64
#  else
#    define UA_GET64(p)     (* (__lzo_ua_volatile const lzo_uint64p) (__lzo_ua_volatile const lzo_voidp) (p))
#    define UA_SET64(p,v)   ((* (__lzo_ua_volatile lzo_uint64p) (__lzo_ua_volatile lzo_voidp) (p)) = (lzo_uint64) (v))
#    define UA_COPY64(d,s)  UA_SET64(d, UA_GET64(s))
#  endif
#endif

#define MEMCPY8_DS(dest,src,len) \
    lzo_memcpy(dest,src,len); dest += len; src += len

#define BZERO8_PTR(s,l,n) \
    lzo_memset((lzo_voidp)(s),0,(lzo_uint)(l)*(n))

#define MEMCPY_DS(dest,src,len) \
    do *dest++ = *src++; while (--len > 0)

LZO_EXTERN(const lzo_bytep) lzo_copyright(void);

#ifndef __LZO_PTR_H
#define __LZO_PTR_H 1

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(lzo_uintptr_t)
#  if (__LZO_MMODEL_HUGE)
#    define lzo_uintptr_t   unsigned long
#  else
#    define lzo_uintptr_t   acc_uintptr_t
#    ifdef __ACC_INTPTR_T_IS_POINTER
#      define __LZO_UINTPTR_T_IS_POINTER 1
#    endif
#  endif
#endif

#if (LZO_ARCH_I086)
#define PTR(a)              ((lzo_bytep) (a))
#define PTR_ALIGNED_4(a)    ((ACC_PTR_FP_OFF(a) & 3) == 0)
#define PTR_ALIGNED2_4(a,b) (((ACC_PTR_FP_OFF(a) | ACC_PTR_FP_OFF(b)) & 3) == 0)
#elif (LZO_MM_PVP)
#define PTR(a)              ((lzo_bytep) (a))
#define PTR_ALIGNED_8(a)    ((((lzo_uintptr_t)(a)) >> 61) == 0)
#define PTR_ALIGNED2_8(a,b) ((((lzo_uintptr_t)(a)|(lzo_uintptr_t)(b)) >> 61) == 0)
#else
#define PTR(a)              ((lzo_uintptr_t) (a))
#define PTR_LINEAR(a)       PTR(a)
#define PTR_ALIGNED_4(a)    ((PTR_LINEAR(a) & 3) == 0)
#define PTR_ALIGNED_8(a)    ((PTR_LINEAR(a) & 7) == 0)
#define PTR_ALIGNED2_4(a,b) (((PTR_LINEAR(a) | PTR_LINEAR(b)) & 3) == 0)
#define PTR_ALIGNED2_8(a,b) (((PTR_LINEAR(a) | PTR_LINEAR(b)) & 7) == 0)
#endif

#define PTR_LT(a,b)         (PTR(a) < PTR(b))
#define PTR_GE(a,b)         (PTR(a) >= PTR(b))
#define PTR_DIFF(a,b)       (PTR(a) - PTR(b))
#define pd(a,b)             ((lzo_uint) ((a)-(b)))

	LZO_EXTERN(lzo_uintptr_t)
		__lzo_ptr_linear(const lzo_voidp ptr);

	typedef union
	{
		char            a_char;
		unsigned char   a_uchar;
		short           a_short;
		unsigned short  a_ushort;
		int             a_int;
		unsigned int    a_uint;
		long            a_long;
		unsigned long   a_ulong;
		lzo_int         a_lzo_int;
		lzo_uint        a_lzo_uint;
		lzo_int32       a_lzo_int32;
		lzo_uint32      a_lzo_uint32;
#if defined(LZO_UINT64_MAX)
		lzo_int64       a_lzo_int64;
		lzo_uint64      a_lzo_uint64;
#endif
		ptrdiff_t       a_ptrdiff_t;
		lzo_uintptr_t   a_lzo_uintptr_t;
		lzo_voidp       a_lzo_voidp;
		void *          a_void_p;
		lzo_bytep       a_lzo_bytep;
		lzo_bytepp      a_lzo_bytepp;
		lzo_uintp       a_lzo_uintp;
		lzo_uint *      a_lzo_uint_p;
		lzo_uint32p     a_lzo_uint32p;
		lzo_uint32 *    a_lzo_uint32_p;
		unsigned char * a_uchar_p;
		char *          a_char_p;
	}
	lzo_full_align_t;

#ifdef __cplusplus
}
#endif

#endif

#ifndef LZO_DETERMINISTIC
#define LZO_DETERMINISTIC 1
#endif

#ifndef LZO_DICT_USE_PTR
#define LZO_DICT_USE_PTR 1
#if 0 && (LZO_ARCH_I086)
#  undef LZO_DICT_USE_PTR
#  define LZO_DICT_USE_PTR 0
#endif
#endif

#if (LZO_DICT_USE_PTR)
#  define lzo_dict_t    const lzo_bytep
#  define lzo_dict_p    lzo_dict_t __LZO_MMODEL *
#else
#  define lzo_dict_t    lzo_uint
#  define lzo_dict_p    lzo_dict_t __LZO_MMODEL *
#endif

#endif

#if !defined(MINILZO_CFG_SKIP_LZO_PTR)

LZO_PUBLIC(lzo_uintptr_t)
__lzo_ptr_linear(const lzo_voidp ptr)
{
	lzo_uintptr_t p;

#if (LZO_ARCH_I086)
	p = (((lzo_uintptr_t)(ACC_PTR_FP_SEG(ptr))) << (16 - ACC_MM_AHSHIFT)) + (ACC_PTR_FP_OFF(ptr));
#elif (LZO_MM_PVP)
	p = (lzo_uintptr_t)(ptr);
	p = (p << 3) | (p >> 61);
#else
	p = (lzo_uintptr_t)PTR_LINEAR(ptr);
#endif

	return p;
}

LZO_PUBLIC(unsigned)
__lzo_align_gap(const lzo_voidp ptr, lzo_uint size)
{
#if defined(__LZO_UINTPTR_T_IS_POINTER)
	size_t n = (size_t)ptr;
	n = (((n + size - 1) / size) * size) - n;
#else
	lzo_uintptr_t p, n;
	p = __lzo_ptr_linear(ptr);
	n = (((p + size - 1) / size) * size) - p;
#endif

	assert(size > 0);
	assert((long)n >= 0);
	assert(n <= size);
	return (unsigned)n;
}

#endif
#if !defined(MINILZO_CFG_SKIP_LZO_UTIL)

/* If you use the LZO library in a product, I would appreciate that you
* keep this copyright string in the executable of your product.
*/

static const char __lzo_copyright[] =
#if !defined(__LZO_IN_MINLZO)
LZO_VERSION_STRING;
#else
"\r\n\n"
"LZO data compression library.\n"
"$Copyright: LZO Copyright (C) 1996-2011 Markus Franz Xaver Johannes Oberhumer\n"
"<markus@oberhumer.com>\n"
"http://www.oberhumer.com $\n\n"
"$Id: LZO version: v" LZO_VERSION_STRING ", " LZO_VERSION_DATE " $\n"
"$Info: " LZO_INFO_STRING " $\n";
#endif

LZO_PUBLIC(const lzo_bytep)
lzo_copyright(void)
{
#if (LZO_OS_DOS16 && LZO_CC_TURBOC)
	return (lzo_voidp)__lzo_copyright;
#else
	return (const lzo_bytep)__lzo_copyright;
#endif
}

LZO_PUBLIC(unsigned)
lzo_version(void)
{
	return LZO_VERSION;
}

LZO_PUBLIC(const char *)
lzo_version_string(void)
{
	return LZO_VERSION_STRING;
}

LZO_PUBLIC(const char *)
lzo_version_date(void)
{
	return LZO_VERSION_DATE;
}

LZO_PUBLIC(const lzo_charp)
_lzo_version_string(void)
{
	return LZO_VERSION_STRING;
}

LZO_PUBLIC(const lzo_charp)
_lzo_version_date(void)
{
	return LZO_VERSION_DATE;
}

#define LZO_BASE 65521u
#define LZO_NMAX 5552

#define LZO_DO1(buf,i)  s1 += buf[i]; s2 += s1
#define LZO_DO2(buf,i)  LZO_DO1(buf,i); LZO_DO1(buf,i+1);
#define LZO_DO4(buf,i)  LZO_DO2(buf,i); LZO_DO2(buf,i+2);
#define LZO_DO8(buf,i)  LZO_DO4(buf,i); LZO_DO4(buf,i+4);
#define LZO_DO16(buf,i) LZO_DO8(buf,i); LZO_DO8(buf,i+8);

LZO_PUBLIC(lzo_uint32)
lzo_adler32(lzo_uint32 adler, const lzo_bytep buf, lzo_uint len)
{
	lzo_uint32 s1 = adler & 0xffff;
	lzo_uint32 s2 = (adler >> 16) & 0xffff;
	unsigned k;

	if (buf == NULL)
		return 1;

	while (len > 0)
	{
		k = len < LZO_NMAX ? (unsigned)len : LZO_NMAX;
		len -= k;
		if (k >= 16) do
		{
			LZO_DO16(buf, 0);
			buf += 16;
			k -= 16;
		} while (k >= 16);
		if (k != 0) do
		{
			s1 += *buf++;
			s2 += s1;
		} while (--k > 0);
		s1 %= LZO_BASE;
		s2 %= LZO_BASE;
	}
	return (s2 << 16) | s1;
}

#undef LZO_DO1
#undef LZO_DO2
#undef LZO_DO4
#undef LZO_DO8
#undef LZO_DO16

#endif
#if !defined(MINILZO_CFG_SKIP_LZO_STRING)
#undef lzo_memcmp
#undef lzo_memcpy
#undef lzo_memmove
#undef lzo_memset
#if !defined(__LZO_MMODEL_HUGE)
#  undef LZO_HAVE_MM_HUGE_PTR
#endif
#define lzo_hsize_t             lzo_uint
#define lzo_hvoid_p             lzo_voidp
#define lzo_hbyte_p             lzo_bytep
#define LZOLIB_PUBLIC(r,f)      LZO_PUBLIC(r) f
#define lzo_hmemcmp             lzo_memcmp
#define lzo_hmemcpy             lzo_memcpy
#define lzo_hmemmove            lzo_memmove
#define lzo_hmemset             lzo_memset
#define __LZOLIB_HMEMCPY_CH_INCLUDED 1
#if !defined(LZOLIB_PUBLIC)
#  define LZOLIB_PUBLIC(r,f)    r __LZOLIB_FUNCNAME(f)
#endif
LZOLIB_PUBLIC(int, lzo_hmemcmp) (const lzo_hvoid_p s1, const lzo_hvoid_p s2, lzo_hsize_t len)
{
#if (LZO_HAVE_MM_HUGE_PTR) || !(HAVE_MEMCMP)
	const lzo_hbyte_p p1 = (const lzo_hbyte_p)s1;
	const lzo_hbyte_p p2 = (const lzo_hbyte_p)s2;
	if __lzo_likely(len > 0) do
	{
		int d = *p1 - *p2;
		if (d != 0)
			return d;
		p1++; p2++;
	} while __lzo_likely(--len > 0);
	return 0;
#else
	return memcmp(s1, s2, len);
#endif
}
LZOLIB_PUBLIC(lzo_hvoid_p, lzo_hmemcpy) (lzo_hvoid_p dest, const lzo_hvoid_p src, lzo_hsize_t len)
{
#if (LZO_HAVE_MM_HUGE_PTR) || !(HAVE_MEMCPY)
	lzo_hbyte_p p1 = (lzo_hbyte_p)dest;
	const lzo_hbyte_p p2 = (const lzo_hbyte_p)src;
	if (!(len > 0) || p1 == p2)
		return dest;
	do
		*p1++ = *p2++;
	while __lzo_likely(--len > 0);
	return dest;
#else
	return memcpy(dest, src, len);
#endif
}
LZOLIB_PUBLIC(lzo_hvoid_p, lzo_hmemmove) (lzo_hvoid_p dest, const lzo_hvoid_p src, lzo_hsize_t len)
{
#if (LZO_HAVE_MM_HUGE_PTR) || !(HAVE_MEMMOVE)
	lzo_hbyte_p p1 = (lzo_hbyte_p)dest;
	const lzo_hbyte_p p2 = (const lzo_hbyte_p)src;
	if (!(len > 0) || p1 == p2)
		return dest;
	if (p1 < p2)
	{
		do
			*p1++ = *p2++;
		while __lzo_likely(--len > 0);
	}
	else
	{
		p1 += len;
		p2 += len;
		do
			*--p1 = *--p2;
		while __lzo_likely(--len > 0);
	}
	return dest;
#else
	return memmove(dest, src, len);
#endif
}
LZOLIB_PUBLIC(lzo_hvoid_p, lzo_hmemset) (lzo_hvoid_p s, int c, lzo_hsize_t len)
{
#if (LZO_HAVE_MM_HUGE_PTR) || !(HAVE_MEMSET)
	lzo_hbyte_p p = (lzo_hbyte_p)s;
	if __lzo_likely(len > 0) do
		*p++ = (unsigned char)c;
	while __lzo_likely(--len > 0);
	return s;
#else
	return memset(s, c, len);
#endif
}
#undef LZOLIB_PUBLIC
#endif
#if !defined(MINILZO_CFG_SKIP_LZO_INIT)

#undef ACCCHK_ASSERT

#if 0
#define WANT_lzo_bitops_clz32 1
#define WANT_lzo_bitops_clz64 1
#endif
#define WANT_lzo_bitops_ctz32 1
#define WANT_lzo_bitops_ctz64 1

#if (defined(_WIN32) || defined(_WIN64)) && ((LZO_CC_INTELC && (__INTEL_COMPILER >= 1000)) || (LZO_CC_MSC && (_MSC_VER >= 1400)))
#include <intrin.h>
#if !defined(lzo_bitops_clz32) && defined(WANT_lzo_bitops_clz32) && 0
#pragma intrinsic(_BitScanReverse)
static __lzo_inline unsigned lzo_bitops_clz32(lzo_uint32 v)
{
	unsigned long r;
	(void)_BitScanReverse(&r, v);
	return (unsigned)r;
}
#define lzo_bitops_clz32 lzo_bitops_clz32
#endif
#if !defined(lzo_bitops_clz64) && defined(WANT_lzo_bitops_clz64) && defined(LZO_UINT64_MAX) && 0
#pragma intrinsic(_BitScanReverse64)
static __lzo_inline unsigned lzo_bitops_clz64(lzo_uint64 v)
{
	unsigned long r;
	(void)_BitScanReverse64(&r, v);
	return (unsigned)r;
}
#define lzo_bitops_clz64 lzo_bitops_clz64
#endif
#if !defined(lzo_bitops_ctz32) && defined(WANT_lzo_bitops_ctz32)
#pragma intrinsic(_BitScanForward)
static __lzo_inline unsigned lzo_bitops_ctz32(lzo_uint32 v)
{
	unsigned long r;
	(void)_BitScanForward(&r, v);
	return (unsigned)r;
}
#define lzo_bitops_ctz32 lzo_bitops_ctz32
#endif
#if !defined(lzo_bitops_ctz64) && defined(WANT_lzo_bitops_ctz64) && defined(LZO_UINT64_MAX)
#pragma intrinsic(_BitScanForward64)
static __lzo_inline unsigned lzo_bitops_ctz64(lzo_uint64 v)
{
	unsigned long r;
	(void)_BitScanForward64(&r, v);
	return (unsigned)r;
}
#define lzo_bitops_ctz64 lzo_bitops_ctz64
#endif

#elif (LZO_CC_CLANG || (LZO_CC_GNUC >= 0x030400ul) || (LZO_CC_INTELC && (__INTEL_COMPILER >= 1000)) || LZO_CC_LLVM)
#if !defined(lzo_bitops_clz32) && defined(WANT_lzo_bitops_clz32)
#define lzo_bitops_clz32(v) ((unsigned) __builtin_clz(v))
#endif
#if !defined(lzo_bitops_clz64) && defined(WANT_lzo_bitops_clz64) && defined(LZO_UINT64_MAX)
#define lzo_bitops_clz64(v) ((unsigned) __builtin_clzll(v))
#endif
#if !defined(lzo_bitops_ctz32) && defined(WANT_lzo_bitops_ctz32)
#define lzo_bitops_ctz32(v) ((unsigned) __builtin_ctz(v))
#endif
#if !defined(lzo_bitops_ctz64) && defined(WANT_lzo_bitops_ctz64) && defined(LZO_UINT64_MAX)
#define lzo_bitops_ctz64(v) ((unsigned) __builtin_ctzll(v))
#endif
#if !defined(lzo_bitops_popcount32) && defined(WANT_lzo_bitops_popcount32)
#define lzo_bitops_popcount32(v) ((unsigned) __builtin_popcount(v))
#endif
#if !defined(lzo_bitops_popcount32) && defined(WANT_lzo_bitops_popcount64) && defined(LZO_UINT64_MAX)
#define lzo_bitops_popcount64(v) ((unsigned) __builtin_popcountll(v))
#endif
#endif

#if 0
#define u2p(ptr,off) ((lzo_voidp) (((lzo_bytep)(lzo_voidp)(ptr)) + (off)))
#else
static __lzo_noinline lzo_voidp u2p(lzo_voidp ptr, lzo_uint off)
{
	return (lzo_voidp)((lzo_bytep)ptr + off);
}
#endif

LZO_PUBLIC(int)
_lzo_config_check(void)
{
	lzo_bool r = 1;
	union {
		lzo_xint a[2]; unsigned char b[2 * LZO_MAX(8, sizeof(lzo_xint))];
#if defined(LZO_UNALIGNED_OK_8)
		lzo_uint64 c[2];
#endif
		unsigned short x[2]; lzo_uint32 y[2]; lzo_uint z[2];
	} u;
	lzo_voidp p;

	u.a[0] = u.a[1] = 0;
	p = u2p(&u, 0);
	r &= ((*(lzo_bytep)p) == 0);
#if !defined(LZO_CFG_NO_CONFIG_CHECK)
#if defined(LZO_ABI_BIG_ENDIAN)
	u.a[0] = u.a[1] = 0; u.b[sizeof(lzo_uint) - 1] = 128;
	p = u2p(&u, 0);
	r &= ((*(lzo_uintp)p) == 128);
#endif
#if defined(LZO_ABI_LITTLE_ENDIAN)
	u.a[0] = u.a[1] = 0; u.b[0] = 128;
	p = u2p(&u, 0);
	r &= ((*(lzo_uintp)p) == 128);
#endif
#if defined(LZO_UNALIGNED_OK_2)
	u.a[0] = u.a[1] = 0;
	u.b[0] = 1; u.b[sizeof(unsigned short) + 1] = 2;
	p = u2p(&u, 1);
	r &= ((*(lzo_ushortp)p) == 0);
#endif
#if defined(LZO_UNALIGNED_OK_4)
	u.a[0] = u.a[1] = 0;
	u.b[0] = 3; u.b[sizeof(lzo_uint32) + 1] = 4;
	p = u2p(&u, 1);
	r &= ((*(lzo_uint32p)p) == 0);
#endif
#if defined(LZO_UNALIGNED_OK_8)
	u.c[0] = u.c[1] = 0;
	u.b[0] = 5; u.b[sizeof(lzo_uint64) + 1] = 6;
	p = u2p(&u, 1);
	r &= ((*(lzo_uint64p)p) == 0);
#endif
#if defined(lzo_bitops_clz32)
	{ unsigned i; lzo_uint32 v = 1;
	for (i = 0; i < 31; i++, v <<= 1)
		r &= lzo_bitops_clz32(v) == 31 - i;
	}
#endif
#if defined(lzo_bitops_clz64)
	{ unsigned i; lzo_uint64 v = 1;
	for (i = 0; i < 63; i++, v <<= 1)
		r &= lzo_bitops_clz64(v) == 63 - i;
	}
#endif
#if defined(lzo_bitops_ctz32)
	{ unsigned i; lzo_uint32 v = 1;
	for (i = 0; i < 31; i++, v <<= 1)
		r &= lzo_bitops_ctz32(v) == i;
	}
#endif
#if defined(lzo_bitops_ctz64)
	{ unsigned i; lzo_uint64 v = 1;
	for (i = 0; i < 63; i++, v <<= 1)
		r &= lzo_bitops_ctz64(v) == i;
	}
#endif
#endif

	return r == 1 ? LZO_E_OK : LZO_E_ERROR;
}

LZO_PUBLIC(int)
__lzo_init_v2(unsigned v, int s1, int s2, int s3, int s4, int s5,
	int s6, int s7, int s8, int s9)
{
	int r;

#undef ACCCHK_ASSERT

	if (v == 0)
		return LZO_E_ERROR;

	r = (s1 == -1 || s1 == (int) sizeof(short)) &&
		(s2 == -1 || s2 == (int) sizeof(int)) &&
		(s3 == -1 || s3 == (int) sizeof(long)) &&
		(s4 == -1 || s4 == (int) sizeof(lzo_uint32)) &&
		(s5 == -1 || s5 == (int) sizeof(lzo_uint)) &&
		(s6 == -1 || s6 == (int)lzo_sizeof_dict_t) &&
		(s7 == -1 || s7 == (int) sizeof(char *)) &&
		(s8 == -1 || s8 == (int) sizeof(lzo_voidp)) &&
		(s9 == -1 || s9 == (int) sizeof(lzo_callback_t));
	if (!r)
		return LZO_E_ERROR;

	r = _lzo_config_check();
	if (r != LZO_E_OK)
		return r;

	return r;
}

#endif

#define LZO1X           1
#define LZO_EOF_CODE    1
#define M2_MAX_OFFSET   0x0800

#if !defined(MINILZO_CFG_SKIP_LZO1X_1_COMPRESS)

#if 1 && defined(UA_GET32)
#undef  LZO_DICT_USE_PTR
#define LZO_DICT_USE_PTR 0
#undef  lzo_dict_t
#define lzo_dict_t unsigned short
#endif

#define LZO_NEED_DICT_H 1
#ifndef D_BITS
#define D_BITS          14
#endif
#define D_INDEX1(d,p)       d = DM(DMUL(0x21,DX3(p,5,5,6)) >> 5)
#define D_INDEX2(d,p)       d = (d & (D_MASK & 0x7ff)) ^ (D_HIGH | 0x1f)
#if 1
#define DINDEX(dv,p)        DM(((DMUL(0x1824429d,dv)) >> (32-D_BITS)))
#else
#define DINDEX(dv,p)        DM((dv) + ((dv) >> (32-D_BITS)))
#endif

#ifndef __LZO_CONFIG1X_H
#define __LZO_CONFIG1X_H 1

#if !defined(LZO1X) && !defined(LZO1Y) && !defined(LZO1Z)
#  define LZO1X 1
#endif

#ifndef LZO_EOF_CODE
#define LZO_EOF_CODE 1
#endif
#undef LZO_DETERMINISTIC

#define M1_MAX_OFFSET   0x0400
#ifndef M2_MAX_OFFSET
#define M2_MAX_OFFSET   0x0800
#endif
#define M3_MAX_OFFSET   0x4000
#define M4_MAX_OFFSET   0xbfff

#define MX_MAX_OFFSET   (M1_MAX_OFFSET + M2_MAX_OFFSET)

#define M1_MIN_LEN      2
#define M1_MAX_LEN      2
#define M2_MIN_LEN      3
#ifndef M2_MAX_LEN
#define M2_MAX_LEN      8
#endif
#define M3_MIN_LEN      3
#define M3_MAX_LEN      33
#define M4_MIN_LEN      3
#define M4_MAX_LEN      9

#define M1_MARKER       0
#define M2_MARKER       64
#define M3_MARKER       32
#define M4_MARKER       16

#ifndef MIN_LOOKAHEAD
#define MIN_LOOKAHEAD       (M2_MAX_LEN + 1)
#endif

#if defined(LZO_NEED_DICT_H)

#ifndef LZO_HASH
#define LZO_HASH            LZO_HASH_LZO_INCREMENTAL_B
#endif
#define DL_MIN_LEN          M2_MIN_LEN

#ifndef __LZO_DICT_H
#define __LZO_DICT_H 1

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(D_BITS) && defined(DBITS)
#  define D_BITS        DBITS
#endif
#if !defined(D_BITS)
#  error "D_BITS is not defined"
#endif
#if (D_BITS < 16)
#  define D_SIZE        LZO_SIZE(D_BITS)
#  define D_MASK        LZO_MASK(D_BITS)
#else
#  define D_SIZE        LZO_USIZE(D_BITS)
#  define D_MASK        LZO_UMASK(D_BITS)
#endif
#define D_HIGH          ((D_MASK >> 1) + 1)

#if !defined(DD_BITS)
#  define DD_BITS       0
#endif
#define DD_SIZE         LZO_SIZE(DD_BITS)
#define DD_MASK         LZO_MASK(DD_BITS)

#if !defined(DL_BITS)
#  define DL_BITS       (D_BITS - DD_BITS)
#endif
#if (DL_BITS < 16)
#  define DL_SIZE       LZO_SIZE(DL_BITS)
#  define DL_MASK       LZO_MASK(DL_BITS)
#else
#  define DL_SIZE       LZO_USIZE(DL_BITS)
#  define DL_MASK       LZO_UMASK(DL_BITS)
#endif

#if (D_BITS != DL_BITS + DD_BITS)
#  error "D_BITS does not match"
#endif
#if (D_BITS < 6 || D_BITS > 18)
#  error "invalid D_BITS"
#endif
#if (DL_BITS < 6 || DL_BITS > 20)
#  error "invalid DL_BITS"
#endif
#if (DD_BITS < 0 || DD_BITS > 6)
#  error "invalid DD_BITS"
#endif

#if !defined(DL_MIN_LEN)
#  define DL_MIN_LEN    3
#endif
#if !defined(DL_SHIFT)
#  define DL_SHIFT      ((DL_BITS + (DL_MIN_LEN - 1)) / DL_MIN_LEN)
#endif

#define LZO_HASH_GZIP                   1
#define LZO_HASH_GZIP_INCREMENTAL       2
#define LZO_HASH_LZO_INCREMENTAL_A      3
#define LZO_HASH_LZO_INCREMENTAL_B      4

#if !defined(LZO_HASH)
#  error "choose a hashing strategy"
#endif

#undef DM
#undef DX

#if (DL_MIN_LEN == 3)
#  define _DV2_A(p,shift1,shift2) \
        (((( (lzo_xint)((p)[0]) << shift1) ^ (p)[1]) << shift2) ^ (p)[2])
#  define _DV2_B(p,shift1,shift2) \
        (((( (lzo_xint)((p)[2]) << shift1) ^ (p)[1]) << shift2) ^ (p)[0])
#  define _DV3_B(p,shift1,shift2,shift3) \
        ((_DV2_B((p)+1,shift1,shift2) << (shift3)) ^ (p)[0])
#elif (DL_MIN_LEN == 2)
#  define _DV2_A(p,shift1,shift2) \
        (( (lzo_xint)(p[0]) << shift1) ^ p[1])
#  define _DV2_B(p,shift1,shift2) \
        (( (lzo_xint)(p[1]) << shift1) ^ p[2])
#else
#  error "invalid DL_MIN_LEN"
#endif
#define _DV_A(p,shift)      _DV2_A(p,shift,shift)
#define _DV_B(p,shift)      _DV2_B(p,shift,shift)
#define DA2(p,s1,s2) \
        (((((lzo_xint)((p)[2]) << (s2)) + (p)[1]) << (s1)) + (p)[0])
#define DS2(p,s1,s2) \
        (((((lzo_xint)((p)[2]) << (s2)) - (p)[1]) << (s1)) - (p)[0])
#define DX2(p,s1,s2) \
        (((((lzo_xint)((p)[2]) << (s2)) ^ (p)[1]) << (s1)) ^ (p)[0])
#define DA3(p,s1,s2,s3) ((DA2((p)+1,s2,s3) << (s1)) + (p)[0])
#define DS3(p,s1,s2,s3) ((DS2((p)+1,s2,s3) << (s1)) - (p)[0])
#define DX3(p,s1,s2,s3) ((DX2((p)+1,s2,s3) << (s1)) ^ (p)[0])
#define DMS(v,s)        ((lzo_uint) (((v) & (D_MASK >> (s))) << (s)))
#define DM(v)           DMS(v,0)

#if (LZO_HASH == LZO_HASH_GZIP)
#  define _DINDEX(dv,p)     (_DV_A((p),DL_SHIFT))

#elif (LZO_HASH == LZO_HASH_GZIP_INCREMENTAL)
#  define __LZO_HASH_INCREMENTAL 1
#  define DVAL_FIRST(dv,p)  dv = _DV_A((p),DL_SHIFT)
#  define DVAL_NEXT(dv,p)   dv = (((dv) << DL_SHIFT) ^ p[2])
#  define _DINDEX(dv,p)     (dv)
#  define DVAL_LOOKAHEAD    DL_MIN_LEN

#elif (LZO_HASH == LZO_HASH_LZO_INCREMENTAL_A)
#  define __LZO_HASH_INCREMENTAL 1
#  define DVAL_FIRST(dv,p)  dv = _DV_A((p),5)
#  define DVAL_NEXT(dv,p) \
                dv ^= (lzo_xint)(p[-1]) << (2*5); dv = (((dv) << 5) ^ p[2])
#  define _DINDEX(dv,p)     ((DMUL(0x9f5f,dv)) >> 5)
#  define DVAL_LOOKAHEAD    DL_MIN_LEN

#elif (LZO_HASH == LZO_HASH_LZO_INCREMENTAL_B)
#  define __LZO_HASH_INCREMENTAL 1
#  define DVAL_FIRST(dv,p)  dv = _DV_B((p),5)
#  define DVAL_NEXT(dv,p) \
                dv ^= p[-1]; dv = (((dv) >> 5) ^ ((lzo_xint)(p[2]) << (2*5)))
#  define _DINDEX(dv,p)     ((DMUL(0x9f5f,dv)) >> 5)
#  define DVAL_LOOKAHEAD    DL_MIN_LEN

#else
#  error "choose a hashing strategy"
#endif

#ifndef DINDEX
#define DINDEX(dv,p)        ((lzo_uint)((_DINDEX(dv,p)) & DL_MASK) << DD_BITS)
#endif
#if !defined(DINDEX1) && defined(D_INDEX1)
#define DINDEX1             D_INDEX1
#endif
#if !defined(DINDEX2) && defined(D_INDEX2)
#define DINDEX2             D_INDEX2
#endif

#if !defined(__LZO_HASH_INCREMENTAL)
#  define DVAL_FIRST(dv,p)  ((void) 0)
#  define DVAL_NEXT(dv,p)   ((void) 0)
#  define DVAL_LOOKAHEAD    0
#endif

#if !defined(DVAL_ASSERT)
#if defined(__LZO_HASH_INCREMENTAL) && !defined(NDEBUG)
#if (LZO_CC_CLANG || (LZO_CC_GNUC >= 0x020700ul) || LZO_CC_LLVM)
	static void __attribute__((__unused__))
#else
	static void
#endif
		DVAL_ASSERT(lzo_xint dv, const lzo_bytep p)
	{
		lzo_xint df;
		DVAL_FIRST(df, (p));
		assert(DINDEX(dv, p) == DINDEX(df, p));
	}
#else
#  define DVAL_ASSERT(dv,p) ((void) 0)
#endif
#endif

#if (LZO_DICT_USE_PTR)
#  define DENTRY(p,in)                          (p)
#  define GINDEX(m_pos,m_off,dict,dindex,in)    m_pos = dict[dindex]
#else
#  define DENTRY(p,in)                          ((lzo_dict_t) pd(p, in))
#  define GINDEX(m_pos,m_off,dict,dindex,in)    m_off = dict[dindex]
#endif

#if (DD_BITS == 0)

#  define UPDATE_D(dict,drun,dv,p,in)       dict[ DINDEX(dv,p) ] = DENTRY(p,in)
#  define UPDATE_I(dict,drun,index,p,in)    dict[index] = DENTRY(p,in)
#  define UPDATE_P(ptr,drun,p,in)           (ptr)[0] = DENTRY(p,in)

#else

#  define UPDATE_D(dict,drun,dv,p,in)   \
        dict[ DINDEX(dv,p) + drun++ ] = DENTRY(p,in); drun &= DD_MASK
#  define UPDATE_I(dict,drun,index,p,in)    \
        dict[ (index) + drun++ ] = DENTRY(p,in); drun &= DD_MASK
#  define UPDATE_P(ptr,drun,p,in)   \
        (ptr) [ drun++ ] = DENTRY(p,in); drun &= DD_MASK

#endif

#if (LZO_DICT_USE_PTR)

#define LZO_CHECK_MPOS_DET(m_pos,m_off,in,ip,max_offset) \
        (m_pos == NULL || (m_off = pd(ip, m_pos)) > max_offset)

#define LZO_CHECK_MPOS_NON_DET(m_pos,m_off,in,ip,max_offset) \
    (BOUNDS_CHECKING_OFF_IN_EXPR(( \
        m_pos = ip - (lzo_uint) PTR_DIFF(ip,m_pos), \
        PTR_LT(m_pos,in) || \
        (m_off = (lzo_uint) PTR_DIFF(ip,m_pos)) == 0 || \
         m_off > max_offset )))

#else

#define LZO_CHECK_MPOS_DET(m_pos,m_off,in,ip,max_offset) \
        (m_off == 0 || \
         ((m_off = pd(ip, in) - m_off) > max_offset) || \
         (m_pos = (ip) - (m_off), 0) )

#define LZO_CHECK_MPOS_NON_DET(m_pos,m_off,in,ip,max_offset) \
        (pd(ip, in) <= m_off || \
         ((m_off = pd(ip, in) - m_off) > max_offset) || \
         (m_pos = (ip) - (m_off), 0) )

#endif

#if (LZO_DETERMINISTIC)
#  define LZO_CHECK_MPOS    LZO_CHECK_MPOS_DET
#else
#  define LZO_CHECK_MPOS    LZO_CHECK_MPOS_NON_DET
#endif

#ifdef __cplusplus
}
#endif

#endif

#endif

#endif

#define LZO_DETERMINISTIC !(LZO_DICT_USE_PTR)

#ifndef DO_COMPRESS
#define DO_COMPRESS     lzo1x_1_compress
#endif

#if 1 && defined(DO_COMPRESS) && !defined(do_compress)
#  define do_compress       LZO_CPP_ECONCAT2(DO_COMPRESS,_core)
#endif

#if defined(UA_GET64)
#  define WANT_lzo_bitops_ctz64 1
#elif defined(UA_GET32)
#  define WANT_lzo_bitops_ctz32 1
#endif

#if (defined(_WIN32) || defined(_WIN64)) && ((LZO_CC_INTELC && (__INTEL_COMPILER >= 1000)) || (LZO_CC_MSC && (_MSC_VER >= 1400)))
#include <intrin.h>
#if !defined(lzo_bitops_clz32) && defined(WANT_lzo_bitops_clz32) && 0
#pragma intrinsic(_BitScanReverse)
static __lzo_inline unsigned lzo_bitops_clz32(lzo_uint32 v)
{
	unsigned long r;
	(void)_BitScanReverse(&r, v);
	return (unsigned)r;
}
#define lzo_bitops_clz32 lzo_bitops_clz32
#endif
#if !defined(lzo_bitops_clz64) && defined(WANT_lzo_bitops_clz64) && defined(LZO_UINT64_MAX) && 0
#pragma intrinsic(_BitScanReverse64)
static __lzo_inline unsigned lzo_bitops_clz64(lzo_uint64 v)
{
	unsigned long r;
	(void)_BitScanReverse64(&r, v);
	return (unsigned)r;
}
#define lzo_bitops_clz64 lzo_bitops_clz64
#endif
#if !defined(lzo_bitops_ctz32) && defined(WANT_lzo_bitops_ctz32)
#pragma intrinsic(_BitScanForward)
static __lzo_inline unsigned lzo_bitops_ctz32(lzo_uint32 v)
{
	unsigned long r;
	(void)_BitScanForward(&r, v);
	return (unsigned)r;
}
#define lzo_bitops_ctz32 lzo_bitops_ctz32
#endif
#if !defined(lzo_bitops_ctz64) && defined(WANT_lzo_bitops_ctz64) && defined(LZO_UINT64_MAX)
#pragma intrinsic(_BitScanForward64)
static __lzo_inline unsigned lzo_bitops_ctz64(lzo_uint64 v)
{
	unsigned long r;
	(void)_BitScanForward64(&r, v);
	return (unsigned)r;
}
#define lzo_bitops_ctz64 lzo_bitops_ctz64
#endif

#elif (LZO_CC_CLANG || (LZO_CC_GNUC >= 0x030400ul) || (LZO_CC_INTELC && (__INTEL_COMPILER >= 1000)) || LZO_CC_LLVM)
#if !defined(lzo_bitops_clz32) && defined(WANT_lzo_bitops_clz32)
#define lzo_bitops_clz32(v) ((unsigned) __builtin_clz(v))
#endif
#if !defined(lzo_bitops_clz64) && defined(WANT_lzo_bitops_clz64) && defined(LZO_UINT64_MAX)
#define lzo_bitops_clz64(v) ((unsigned) __builtin_clzll(v))
#endif
#if !defined(lzo_bitops_ctz32) && defined(WANT_lzo_bitops_ctz32)
#define lzo_bitops_ctz32(v) ((unsigned) __builtin_ctz(v))
#endif
#if !defined(lzo_bitops_ctz64) && defined(WANT_lzo_bitops_ctz64) && defined(LZO_UINT64_MAX)
#define lzo_bitops_ctz64(v) ((unsigned) __builtin_ctzll(v))
#endif
#if !defined(lzo_bitops_popcount32) && defined(WANT_lzo_bitops_popcount32)
#define lzo_bitops_popcount32(v) ((unsigned) __builtin_popcount(v))
#endif
#if !defined(lzo_bitops_popcount32) && defined(WANT_lzo_bitops_popcount64) && defined(LZO_UINT64_MAX)
#define lzo_bitops_popcount64(v) ((unsigned) __builtin_popcountll(v))
#endif
#endif

static __lzo_noinline lzo_uint
do_compress(const lzo_bytep in, lzo_uint  in_len,
	lzo_bytep out, lzo_uintp out_len,
	lzo_uint  ti, lzo_voidp wrkmem)
{
	register const lzo_bytep ip;
	lzo_bytep op;
	const lzo_bytep const in_end = in + in_len;
	const lzo_bytep const ip_end = in + in_len - 20;
	const lzo_bytep ii;
	lzo_dict_p const dict = (lzo_dict_p)wrkmem;

	op = out;
	ip = in;
	ii = ip - ti;

	ip += ti < 4 ? 4 - ti : 0;
	for (;;)
	{
		const lzo_bytep m_pos;
#if !(LZO_DETERMINISTIC)
		LZO_DEFINE_UNINITIALIZED_VAR(lzo_uint, m_off, 0);
		lzo_uint m_len;
		lzo_uint dindex;
	next:
		if __lzo_unlikely(ip >= ip_end)
			break;
		DINDEX1(dindex, ip);
		GINDEX(m_pos, m_off, dict, dindex, in);
		if (LZO_CHECK_MPOS_NON_DET(m_pos, m_off, in, ip, M4_MAX_OFFSET))
			goto literal;
#if 1
		if (m_off <= M2_MAX_OFFSET || m_pos[3] == ip[3])
			goto try_match;
		DINDEX2(dindex, ip);
#endif
		GINDEX(m_pos, m_off, dict, dindex, in);
		if (LZO_CHECK_MPOS_NON_DET(m_pos, m_off, in, ip, M4_MAX_OFFSET))
			goto literal;
		if (m_off <= M2_MAX_OFFSET || m_pos[3] == ip[3])
			goto try_match;
		goto literal;

	try_match:
#if defined(UA_GET32)
		if (UA_GET32(m_pos) != UA_GET32(ip))
#else
		if (m_pos[0] != ip[0] || m_pos[1] != ip[1] || m_pos[2] != ip[2] || m_pos[3] != ip[3])
#endif
		{
			literal:
			UPDATE_I(dict, 0, dindex, ip, in);
			ip += 1 + ((ip - ii) >> 5);
			continue;
		}
		UPDATE_I(dict, 0, dindex, ip, in);
#else
		lzo_uint m_off;
		lzo_uint m_len;
		{
			lzo_uint32 dv;
			lzo_uint dindex;
			literal:
			ip += 1 + ((ip - ii) >> 5);
		next:
			if __lzo_unlikely(ip >= ip_end)
				break;
			dv = UA_GET32(ip);
			dindex = DINDEX(dv, ip);
			GINDEX(m_off, m_pos, in + dict, dindex, in);
			UPDATE_I(dict, 0, dindex, ip, in);
			if __lzo_unlikely(dv != UA_GET32(m_pos))
				goto literal;
		}
#endif

		{
			register lzo_uint t = pd(ip, ii);
			if (t != 0)
			{
				if (t <= 3)
				{
					op[-2] |= LZO_BYTE(t);
#if defined(UA_COPY32)
					UA_COPY32(op, ii);
					op += t;
#else
					{ do *op++ = *ii++; while (--t > 0); }
#endif
				}
#if defined(UA_COPY32) || defined(UA_COPY64)
				else if (t <= 16)
				{
					*op++ = LZO_BYTE(t - 3);
#if defined(UA_COPY64)
					UA_COPY64(op, ii);
					UA_COPY64(op + 8, ii + 8);
#else
					UA_COPY32(op, ii);
					UA_COPY32(op + 4, ii + 4);
					UA_COPY32(op + 8, ii + 8);
					UA_COPY32(op + 12, ii + 12);
#endif
					op += t;
				}
#endif
				else
				{
					if (t <= 18)
						*op++ = LZO_BYTE(t - 3);
					else
					{
						register lzo_uint tt = t - 18;
						*op++ = 0;
						while __lzo_unlikely(tt > 255)
						{
							tt -= 255;
#if 1 && (LZO_CC_MSC && (_MSC_VER >= 1400))
							* (volatile unsigned char *)op++ = 0;
#else
							*op++ = 0;
#endif
						}
						assert(tt > 0);
						*op++ = LZO_BYTE(tt);
					}
#if defined(UA_COPY32) || defined(UA_COPY64)
					do {
#if defined(UA_COPY64)
						UA_COPY64(op, ii);
						UA_COPY64(op + 8, ii + 8);
#else
						UA_COPY32(op, ii);
						UA_COPY32(op + 4, ii + 4);
						UA_COPY32(op + 8, ii + 8);
						UA_COPY32(op + 12, ii + 12);
#endif
						op += 16; ii += 16; t -= 16;
					} while (t >= 16); if (t > 0)
#endif
					{
						do *op++ = *ii++; while (--t > 0);
					}
				}
			}
		}
		m_len = 4;
		{
#if defined(UA_GET64)
			lzo_uint64 v;
			v = UA_GET64(ip + m_len) ^ UA_GET64(m_pos + m_len);
			if __lzo_unlikely(v == 0) {
				do {
					m_len += 8;
					v = UA_GET64(ip + m_len) ^ UA_GET64(m_pos + m_len);
					if __lzo_unlikely(ip + m_len >= ip_end)
						goto m_len_done;
				} while (v == 0);
			}
#if (LZO_ABI_LITTLE_ENDIAN) && defined(lzo_bitops_ctz64)
			m_len += lzo_bitops_ctz64(v) / CHAR_BIT;
#elif (LZO_ABI_LITTLE_ENDIAN)
			if ((v & UCHAR_MAX) == 0) do {
				v >>= CHAR_BIT;
				m_len += 1;
			} while ((v & UCHAR_MAX) == 0);
#else
			if (ip[m_len] == m_pos[m_len]) do {
				m_len += 1;
			} while (ip[m_len] == m_pos[m_len]);
#endif
#elif defined(UA_GET32)
			lzo_uint32 v;
			v = UA_GET32(ip + m_len) ^ UA_GET32(m_pos + m_len);
			if __lzo_unlikely(v == 0) {
				do {
					m_len += 4;
					v = UA_GET32(ip + m_len) ^ UA_GET32(m_pos + m_len);
					if __lzo_unlikely(ip + m_len >= ip_end)
						goto m_len_done;
				} while (v == 0);
			}
#if (LZO_ABI_LITTLE_ENDIAN) && defined(lzo_bitops_ctz32)
			m_len += lzo_bitops_ctz32(v) / CHAR_BIT;
#elif (LZO_ABI_LITTLE_ENDIAN)
			if ((v & UCHAR_MAX) == 0) do {
				v >>= CHAR_BIT;
				m_len += 1;
			} while ((v & UCHAR_MAX) == 0);
#else
			if (ip[m_len] == m_pos[m_len]) do {
				m_len += 1;
			} while (ip[m_len] == m_pos[m_len]);
#endif
#else
			if __lzo_unlikely(ip[m_len] == m_pos[m_len]) {
				do {
					m_len += 1;
					if __lzo_unlikely(ip + m_len >= ip_end)
						goto m_len_done;
				} while (ip[m_len] == m_pos[m_len]);
			}
#endif
		}
	m_len_done:
		m_off = pd(ip, m_pos);
		ip += m_len;
		ii = ip;
		if (m_len <= M2_MAX_LEN && m_off <= M2_MAX_OFFSET)
		{
			m_off -= 1;
#if defined(LZO1X)
			*op++ = LZO_BYTE(((m_len - 1) << 5) | ((m_off & 7) << 2));
			*op++ = LZO_BYTE(m_off >> 3);
#elif defined(LZO1Y)
			*op++ = LZO_BYTE(((m_len + 1) << 4) | ((m_off & 3) << 2));
			*op++ = LZO_BYTE(m_off >> 2);
#endif
		}
		else if (m_off <= M3_MAX_OFFSET)
		{
			m_off -= 1;
			if (m_len <= M3_MAX_LEN)
				*op++ = LZO_BYTE(M3_MARKER | (m_len - 2));
			else
			{
				m_len -= M3_MAX_LEN;
				*op++ = M3_MARKER | 0;
				while __lzo_unlikely(m_len > 255)
				{
					m_len -= 255;
#if 1 && (LZO_CC_MSC && (_MSC_VER >= 1400))
					* (volatile unsigned char *)op++ = 0;
#else
					*op++ = 0;
#endif
				}
				*op++ = LZO_BYTE(m_len);
			}
			*op++ = LZO_BYTE(m_off << 2);
			*op++ = LZO_BYTE(m_off >> 6);
		}
		else
		{
			m_off -= 0x4000;
			if (m_len <= M4_MAX_LEN)
				*op++ = LZO_BYTE(M4_MARKER | ((m_off >> 11) & 8) | (m_len - 2));
			else
			{
				m_len -= M4_MAX_LEN;
				*op++ = LZO_BYTE(M4_MARKER | ((m_off >> 11) & 8));
				while __lzo_unlikely(m_len > 255)
				{
					m_len -= 255;
#if 1 && (LZO_CC_MSC && (_MSC_VER >= 1400))
					* (volatile unsigned char *)op++ = 0;
#else
					*op++ = 0;
#endif
				}
				*op++ = LZO_BYTE(m_len);
			}
			*op++ = LZO_BYTE(m_off << 2);
			*op++ = LZO_BYTE(m_off >> 6);
		}
		goto next;
	}

	*out_len = pd(op, out);
	return pd(in_end, ii);
}

LZO_PUBLIC(int)
DO_COMPRESS(const lzo_bytep in, lzo_uint  in_len,
	lzo_bytep out, lzo_uintp out_len,
	lzo_voidp wrkmem)
{
	const lzo_bytep ip = in;
	lzo_bytep op = out;
	lzo_uint l = in_len;
	lzo_uint t = 0;

	while (l > 20)
	{
		lzo_uint ll = l;
		lzo_uintptr_t ll_end;
#if 0 || (LZO_DETERMINISTIC)
		ll = LZO_MIN(ll, 49152);
#endif
		ll_end = (lzo_uintptr_t)ip + ll;
		if ((ll_end + ((t + ll) >> 5)) <= ll_end || (const lzo_bytep)(ll_end + ((t + ll) >> 5)) <= ip + ll)
			break;
#if (LZO_DETERMINISTIC)
		lzo_memset(wrkmem, 0, ((lzo_uint)1 << D_BITS) * sizeof(lzo_dict_t));
#endif
		t = do_compress(ip, ll, op, out_len, t, wrkmem);
		ip += ll;
		op += *out_len;
		l -= ll;
	}
	t += l;

	if (t > 0)
	{
		const lzo_bytep ii = in + in_len - t;

		if (op == out && t <= 238)
			*op++ = LZO_BYTE(17 + t);
		else if (t <= 3)
			op[-2] |= LZO_BYTE(t);
		else if (t <= 18)
			*op++ = LZO_BYTE(t - 3);
		else
		{
			lzo_uint tt = t - 18;

			*op++ = 0;
			while (tt > 255)
			{
				tt -= 255;
#if 1 && (LZO_CC_MSC && (_MSC_VER >= 1400))

				* (volatile unsigned char *)op++ = 0;
#else
				*op++ = 0;
#endif
			}
			assert(tt > 0);
			*op++ = LZO_BYTE(tt);
		}
		do *op++ = *ii++; while (--t > 0);
	}

	*op++ = M4_MARKER | 1;
	*op++ = 0;
	*op++ = 0;

	*out_len = pd(op, out);
	return LZO_E_OK;
}

#endif

#undef do_compress
#undef DO_COMPRESS
#undef LZO_HASH

#undef LZO_TEST_OVERRUN
#undef DO_DECOMPRESS
#define DO_DECOMPRESS       lzo1x_decompress

#if !defined(MINILZO_CFG_SKIP_LZO1X_DECOMPRESS)

#if defined(LZO_TEST_OVERRUN)
#  if !defined(LZO_TEST_OVERRUN_INPUT)
#    define LZO_TEST_OVERRUN_INPUT       2
#  endif
#  if !defined(LZO_TEST_OVERRUN_OUTPUT)
#    define LZO_TEST_OVERRUN_OUTPUT      2
#  endif
#  if !defined(LZO_TEST_OVERRUN_LOOKBEHIND)
#    define LZO_TEST_OVERRUN_LOOKBEHIND  1
#  endif
#endif

#undef TEST_IP
#undef TEST_OP
#undef TEST_LB
#undef TEST_LBO
#undef NEED_IP
#undef NEED_OP
#undef HAVE_TEST_IP
#undef HAVE_TEST_OP
#undef HAVE_NEED_IP
#undef HAVE_NEED_OP
#undef HAVE_ANY_IP
#undef HAVE_ANY_OP

#if defined(LZO_TEST_OVERRUN_INPUT)
#  if (LZO_TEST_OVERRUN_INPUT >= 1)
#    define TEST_IP             (ip < ip_end)
#  endif
#  if (LZO_TEST_OVERRUN_INPUT >= 2)
#    define NEED_IP(x) \
            if ((lzo_uint)(ip_end - ip) < (lzo_uint)(x))  goto input_overrun
#  endif
#endif

#if defined(LZO_TEST_OVERRUN_OUTPUT)
#  if (LZO_TEST_OVERRUN_OUTPUT >= 1)
#    define TEST_OP             (op <= op_end)
#  endif
#  if (LZO_TEST_OVERRUN_OUTPUT >= 2)
#    undef TEST_OP
#    define NEED_OP(x) \
            if ((lzo_uint)(op_end - op) < (lzo_uint)(x))  goto output_overrun
#  endif
#endif

#if defined(LZO_TEST_OVERRUN_LOOKBEHIND)
#  define TEST_LB(m_pos)        if (m_pos < out || m_pos >= op) goto lookbehind_overrun
#  define TEST_LBO(m_pos,o)     if (m_pos < out || m_pos >= op - (o)) goto lookbehind_overrun
#else
#  define TEST_LB(m_pos)        ((void) 0)
#  define TEST_LBO(m_pos,o)     ((void) 0)
#endif

#if !defined(LZO_EOF_CODE) && !defined(TEST_IP)
#  define TEST_IP               (ip < ip_end)
#endif

#if defined(TEST_IP)
#  define HAVE_TEST_IP 1
#else
#  define TEST_IP               1
#endif
#if defined(TEST_OP)
#  define HAVE_TEST_OP 1
#else
#  define TEST_OP               1
#endif

#if defined(NEED_IP)
#  define HAVE_NEED_IP 1
#else
#  define NEED_IP(x)            ((void) 0)
#endif
#if defined(NEED_OP)
#  define HAVE_NEED_OP 1
#else
#  define NEED_OP(x)            ((void) 0)
#endif

#if defined(HAVE_TEST_IP) || defined(HAVE_NEED_IP)
#  define HAVE_ANY_IP 1
#endif
#if defined(HAVE_TEST_OP) || defined(HAVE_NEED_OP)
#  define HAVE_ANY_OP 1
#endif

#if defined(DO_DECOMPRESS)
LZO_PUBLIC(int)
DO_DECOMPRESS(const lzo_bytep in, lzo_uint  in_len,
	lzo_bytep out, lzo_uintp out_len,
	lzo_voidp wrkmem)
#endif
{
	register lzo_bytep op;
	register const lzo_bytep ip;
	register lzo_uint t;
#if defined(COPY_DICT)
	lzo_uint m_off;
	const lzo_bytep dict_end;
#else
	register const lzo_bytep m_pos;
#endif

	const lzo_bytep const ip_end = in + in_len;
#if defined(HAVE_ANY_OP)
	lzo_bytep const op_end = out + *out_len;
#endif
#if defined(LZO1Z)
	lzo_uint last_m_off = 0;
#endif

	LZO_UNUSED(wrkmem);

#if defined(COPY_DICT)
	if (dict)
	{
		if (dict_len > M4_MAX_OFFSET)
		{
			dict += dict_len - M4_MAX_OFFSET;
			dict_len = M4_MAX_OFFSET;
		}
		dict_end = dict + dict_len;
	}
	else
	{
		dict_len = 0;
		dict_end = NULL;
	}
#endif

	*out_len = 0;

	op = out;
	ip = in;

	if (*ip > 17)
	{
		t = *ip++ - 17;
		if (t < 4)
			goto match_next;
		assert(t > 0); NEED_OP(t); NEED_IP(t + 1);
		do *op++ = *ip++; while (--t > 0);
		goto first_literal_run;
	}

	while (TEST_IP && TEST_OP)
	{
		t = *ip++;
		if (t >= 16)
			goto match;
		if (t == 0)
		{
			NEED_IP(1);
			while (*ip == 0)
			{
				t += 255;
				ip++;
				NEED_IP(1);
			}
			t += 15 + *ip++;
		}
		assert(t > 0); NEED_OP(t + 3); NEED_IP(t + 4);
#if defined(LZO_UNALIGNED_OK_8) && defined(LZO_UNALIGNED_OK_4)
		t += 3;
		if (t >= 8) do
		{
			UA_COPY64(op, ip);
			op += 8; ip += 8; t -= 8;
		} while (t >= 8);
		if (t >= 4)
		{
			UA_COPY32(op, ip);
			op += 4; ip += 4; t -= 4;
		}
		if (t > 0)
		{
			*op++ = *ip++;
			if (t > 1) { *op++ = *ip++; if (t > 2) { *op++ = *ip++; } }
		}
#elif defined(LZO_UNALIGNED_OK_4) || defined(LZO_ALIGNED_OK_4)
#if !defined(LZO_UNALIGNED_OK_4)
		if (PTR_ALIGNED2_4(op, ip))
		{
#endif
			UA_COPY32(op, ip);
			op += 4; ip += 4;
			if (--t > 0)
			{
				if (t >= 4)
				{
					do {
						UA_COPY32(op, ip);
						op += 4; ip += 4; t -= 4;
					} while (t >= 4);
					if (t > 0) do *op++ = *ip++; while (--t > 0);
				}
				else
					do *op++ = *ip++; while (--t > 0);
			}
#if !defined(LZO_UNALIGNED_OK_4)
		}
		else
#endif
#endif
#if !defined(LZO_UNALIGNED_OK_4) && !defined(LZO_UNALIGNED_OK_8)
		{
			*op++ = *ip++; *op++ = *ip++; *op++ = *ip++;
			do *op++ = *ip++; while (--t > 0);
		}
#endif

	first_literal_run:

		t = *ip++;
		if (t >= 16)
			goto match;
#if defined(COPY_DICT)
#if defined(LZO1Z)
		m_off = (1 + M2_MAX_OFFSET) + (t << 6) + (*ip++ >> 2);
		last_m_off = m_off;
#else
		m_off = (1 + M2_MAX_OFFSET) + (t >> 2) + (*ip++ << 2);
#endif
		NEED_OP(3);
		t = 3; COPY_DICT(t, m_off)
#else
#if defined(LZO1Z)
		t = (1 + M2_MAX_OFFSET) + (t << 6) + (*ip++ >> 2);
		m_pos = op - t;
		last_m_off = t;
#else
		m_pos = op - (1 + M2_MAX_OFFSET);
		m_pos -= t >> 2;
		m_pos -= *ip++ << 2;
#endif
		TEST_LB(m_pos); NEED_OP(3);
		*op++ = *m_pos++; *op++ = *m_pos++; *op++ = *m_pos;
#endif
		goto match_done;

		do {
		match:
			if (t >= 64)
			{
#if defined(COPY_DICT)
#if defined(LZO1X)
				m_off = 1 + ((t >> 2) & 7) + (*ip++ << 3);
				t = (t >> 5) - 1;
#elif defined(LZO1Y)
				m_off = 1 + ((t >> 2) & 3) + (*ip++ << 2);
				t = (t >> 4) - 3;
#elif defined(LZO1Z)
				m_off = t & 0x1f;
				if (m_off >= 0x1c)
					m_off = last_m_off;
				else
				{
					m_off = 1 + (m_off << 6) + (*ip++ >> 2);
					last_m_off = m_off;
				}
				t = (t >> 5) - 1;
#endif
#else
#if defined(LZO1X)
				m_pos = op - 1;
				m_pos -= (t >> 2) & 7;
				m_pos -= *ip++ << 3;
				t = (t >> 5) - 1;
#elif defined(LZO1Y)
				m_pos = op - 1;
				m_pos -= (t >> 2) & 3;
				m_pos -= *ip++ << 2;
				t = (t >> 4) - 3;
#elif defined(LZO1Z)
				{
					lzo_uint off = t & 0x1f;
					m_pos = op;
					if (off >= 0x1c)
					{
						assert(last_m_off > 0);
						m_pos -= last_m_off;
					}
					else
					{
						off = 1 + (off << 6) + (*ip++ >> 2);
						m_pos -= off;
						last_m_off = off;
					}
				}
				t = (t >> 5) - 1;
#endif
				TEST_LB(m_pos); assert(t > 0); NEED_OP(t + 3 - 1);
				goto copy_match;
#endif
			}
			else if (t >= 32)
			{
				t &= 31;
				if (t == 0)
				{
					NEED_IP(1);
					while (*ip == 0)
					{
						t += 255;
						ip++;
						NEED_IP(1);
					}
					t += 31 + *ip++;
				}
#if defined(COPY_DICT)
#if defined(LZO1Z)
				m_off = 1 + (ip[0] << 6) + (ip[1] >> 2);
				last_m_off = m_off;
#else
				m_off = 1 + (ip[0] >> 2) + (ip[1] << 6);
#endif
#else
#if defined(LZO1Z)
				{
					lzo_uint off = 1 + (ip[0] << 6) + (ip[1] >> 2);
					m_pos = op - off;
					last_m_off = off;
				}
#elif defined(LZO_UNALIGNED_OK_2) && defined(LZO_ABI_LITTLE_ENDIAN)
				m_pos = op - 1;
				m_pos -= UA_GET16(ip) >> 2;
#else
				m_pos = op - 1;
				m_pos -= (ip[0] >> 2) + (ip[1] << 6);
#endif
#endif
				ip += 2;
			}
			else if (t >= 16)
			{
#if defined(COPY_DICT)
				m_off = (t & 8) << 11;
#else
				m_pos = op;
				m_pos -= (t & 8) << 11;
#endif
				t &= 7;
				if (t == 0)
				{
					NEED_IP(1);
					while (*ip == 0)
					{
						t += 255;
						ip++;
						NEED_IP(1);
					}
					t += 7 + *ip++;
				}
#if defined(COPY_DICT)
#if defined(LZO1Z)
				m_off += (ip[0] << 6) + (ip[1] >> 2);
#else
				m_off += (ip[0] >> 2) + (ip[1] << 6);
#endif
				ip += 2;
				if (m_off == 0)
					goto eof_found;
				m_off += 0x4000;
#if defined(LZO1Z)
				last_m_off = m_off;
#endif
#else
#if defined(LZO1Z)
				m_pos -= (ip[0] << 6) + (ip[1] >> 2);
#elif defined(LZO_UNALIGNED_OK_2) && defined(LZO_ABI_LITTLE_ENDIAN)
				m_pos -= UA_GET16(ip) >> 2;
#else
				m_pos -= (ip[0] >> 2) + (ip[1] << 6);
#endif
				ip += 2;
				if (m_pos == op)
					goto eof_found;
				m_pos -= 0x4000;
#if defined(LZO1Z)
				last_m_off = pd((const lzo_bytep)op, m_pos);
#endif
#endif
			}
			else
			{
#if defined(COPY_DICT)
#if defined(LZO1Z)
				m_off = 1 + (t << 6) + (*ip++ >> 2);
				last_m_off = m_off;
#else
				m_off = 1 + (t >> 2) + (*ip++ << 2);
#endif
				NEED_OP(2);
				t = 2; COPY_DICT(t, m_off)
#else
#if defined(LZO1Z)
				t = 1 + (t << 6) + (*ip++ >> 2);
				m_pos = op - t;
				last_m_off = t;
#else
				m_pos = op - 1;
				m_pos -= t >> 2;
				m_pos -= *ip++ << 2;
#endif
				TEST_LB(m_pos); NEED_OP(2);
				*op++ = *m_pos++; *op++ = *m_pos;
#endif
				goto match_done;
			}

#if defined(COPY_DICT)

			NEED_OP(t + 3 - 1);
			t += 3 - 1; COPY_DICT(t, m_off)

#else

			TEST_LB(m_pos); assert(t > 0); NEED_OP(t + 3 - 1);
#if defined(LZO_UNALIGNED_OK_8) && defined(LZO_UNALIGNED_OK_4)
			if (op - m_pos >= 8)
			{
				t += (3 - 1);
				if (t >= 8) do
				{
					UA_COPY64(op, m_pos);
					op += 8; m_pos += 8; t -= 8;
				} while (t >= 8);
				if (t >= 4)
				{
					UA_COPY32(op, m_pos);
					op += 4; m_pos += 4; t -= 4;
				}
				if (t > 0)
				{
					*op++ = m_pos[0];
					if (t > 1) { *op++ = m_pos[1]; if (t > 2) { *op++ = m_pos[2]; } }
				}
			}
			else
#elif defined(LZO_UNALIGNED_OK_4) || defined(LZO_ALIGNED_OK_4)
#if !defined(LZO_UNALIGNED_OK_4)
			if (t >= 2 * 4 - (3 - 1) && PTR_ALIGNED2_4(op, m_pos))
			{
				assert((op - m_pos) >= 4);
#else
			if (t >= 2 * 4 - (3 - 1) && (op - m_pos) >= 4)
			{
#endif
				UA_COPY32(op, m_pos);
				op += 4; m_pos += 4; t -= 4 - (3 - 1);
				do {
					UA_COPY32(op, m_pos);
					op += 4; m_pos += 4; t -= 4;
				} while (t >= 4);
				if (t > 0) do *op++ = *m_pos++; while (--t > 0);
			}
			else
#endif
			{
			copy_match:
				*op++ = *m_pos++; *op++ = *m_pos++;
				do *op++ = *m_pos++; while (--t > 0);
			}

#endif

		match_done:
#if defined(LZO1Z)
			t = ip[-1] & 3;
#else
			t = ip[-2] & 3;
#endif
			if (t == 0)
				break;

		match_next:
			assert(t > 0); assert(t < 4); NEED_OP(t); NEED_IP(t + 1);
#if 0
			do *op++ = *ip++; while (--t > 0);
#else
			*op++ = *ip++;
			if (t > 1) { *op++ = *ip++; if (t > 2) { *op++ = *ip++; } }
#endif
			t = *ip++;
			} while (TEST_IP && TEST_OP);
		}

#if defined(HAVE_TEST_IP) || defined(HAVE_TEST_OP)
	*out_len = pd(op, out);
	return LZO_E_EOF_NOT_FOUND;
#endif

eof_found:
	assert(t == 1);
	*out_len = pd(op, out);
	return (ip == ip_end ? LZO_E_OK :
		(ip < ip_end ? LZO_E_INPUT_NOT_CONSUMED : LZO_E_INPUT_OVERRUN));

#if defined(HAVE_NEED_IP)
	input_overrun:
				 *out_len = pd(op, out);
				 return LZO_E_INPUT_OVERRUN;
#endif

#if defined(HAVE_NEED_OP)
				 output_overrun:
							   *out_len = pd(op, out);
							   return LZO_E_OUTPUT_OVERRUN;
#endif

#if defined(LZO_TEST_OVERRUN_LOOKBEHIND)
							   lookbehind_overrun:
												 *out_len = pd(op, out);
												 return LZO_E_LOOKBEHIND_OVERRUN;
#endif
	}

#endif

#define LZO_TEST_OVERRUN 1
#undef DO_DECOMPRESS
#define DO_DECOMPRESS       lzo1x_decompress_safe

#if !defined(MINILZO_CFG_SKIP_LZO1X_DECOMPRESS_SAFE)

#if defined(LZO_TEST_OVERRUN)
#  if !defined(LZO_TEST_OVERRUN_INPUT)
#    define LZO_TEST_OVERRUN_INPUT       2
#  endif
#  if !defined(LZO_TEST_OVERRUN_OUTPUT)
#    define LZO_TEST_OVERRUN_OUTPUT      2
#  endif
#  if !defined(LZO_TEST_OVERRUN_LOOKBEHIND)
#    define LZO_TEST_OVERRUN_LOOKBEHIND  1
#  endif
#endif

#undef TEST_IP
#undef TEST_OP
#undef TEST_LB
#undef TEST_LBO
#undef NEED_IP
#undef NEED_OP
#undef HAVE_TEST_IP
#undef HAVE_TEST_OP
#undef HAVE_NEED_IP
#undef HAVE_NEED_OP
#undef HAVE_ANY_IP
#undef HAVE_ANY_OP

#if defined(LZO_TEST_OVERRUN_INPUT)
#  if (LZO_TEST_OVERRUN_INPUT >= 1)
#    define TEST_IP             (ip < ip_end)
#  endif
#  if (LZO_TEST_OVERRUN_INPUT >= 2)
#    define NEED_IP(x) \
            if ((lzo_uint)(ip_end - ip) < (lzo_uint)(x))  goto input_overrun
#  endif
#endif

#if defined(LZO_TEST_OVERRUN_OUTPUT)
#  if (LZO_TEST_OVERRUN_OUTPUT >= 1)
#    define TEST_OP             (op <= op_end)
#  endif
#  if (LZO_TEST_OVERRUN_OUTPUT >= 2)
#    undef TEST_OP
#    define NEED_OP(x) \
            if ((lzo_uint)(op_end - op) < (lzo_uint)(x))  goto output_overrun
#  endif
#endif

#if defined(LZO_TEST_OVERRUN_LOOKBEHIND)
#  define TEST_LB(m_pos)        if (m_pos < out || m_pos >= op) goto lookbehind_overrun
#  define TEST_LBO(m_pos,o)     if (m_pos < out || m_pos >= op - (o)) goto lookbehind_overrun
#else
#  define TEST_LB(m_pos)        ((void) 0)
#  define TEST_LBO(m_pos,o)     ((void) 0)
#endif

#if !defined(LZO_EOF_CODE) && !defined(TEST_IP)
#  define TEST_IP               (ip < ip_end)
#endif

#if defined(TEST_IP)
#  define HAVE_TEST_IP 1
#else
#  define TEST_IP               1
#endif
#if defined(TEST_OP)
#  define HAVE_TEST_OP 1
#else
#  define TEST_OP               1
#endif

#if defined(NEED_IP)
#  define HAVE_NEED_IP 1
#else
#  define NEED_IP(x)            ((void) 0)
#endif
#if defined(NEED_OP)
#  define HAVE_NEED_OP 1
#else
#  define NEED_OP(x)            ((void) 0)
#endif

#if defined(HAVE_TEST_IP) || defined(HAVE_NEED_IP)
#  define HAVE_ANY_IP 1
#endif
#if defined(HAVE_TEST_OP) || defined(HAVE_NEED_OP)
#  define HAVE_ANY_OP 1
#endif

#if defined(DO_DECOMPRESS)
LZO_PUBLIC(int)
DO_DECOMPRESS(const lzo_bytep in, lzo_uint  in_len,
	lzo_bytep out, lzo_uintp out_len,
	lzo_voidp wrkmem)
#endif
{
	register lzo_bytep op;
	register const lzo_bytep ip;
	register lzo_uint t;
#if defined(COPY_DICT)
	lzo_uint m_off;
	const lzo_bytep dict_end;
#else
	register const lzo_bytep m_pos;
#endif

	const lzo_bytep const ip_end = in + in_len;
#if defined(HAVE_ANY_OP)
	lzo_bytep const op_end = out + *out_len;
#endif
#if defined(LZO1Z)
	lzo_uint last_m_off = 0;
#endif

	LZO_UNUSED(wrkmem);

#if defined(COPY_DICT)
	if (dict)
	{
		if (dict_len > M4_MAX_OFFSET)
		{
			dict += dict_len - M4_MAX_OFFSET;
			dict_len = M4_MAX_OFFSET;
		}
		dict_end = dict + dict_len;
	}
	else
	{
		dict_len = 0;
		dict_end = NULL;
	}
#endif

	*out_len = 0;

	op = out;
	ip = in;

	if (*ip > 17)
	{
		t = *ip++ - 17;
		if (t < 4)
			goto match_next;
		assert(t > 0); NEED_OP(t); NEED_IP(t + 1);
		do *op++ = *ip++; while (--t > 0);
		goto first_literal_run;
	}

	while (TEST_IP && TEST_OP)
	{
		t = *ip++;
		if (t >= 16)
			goto match;
		if (t == 0)
		{
			NEED_IP(1);
			while (*ip == 0)
			{
				t += 255;
				ip++;
				NEED_IP(1);
			}
			t += 15 + *ip++;
		}
		assert(t > 0); NEED_OP(t + 3); NEED_IP(t + 4);
#if defined(LZO_UNALIGNED_OK_8) && defined(LZO_UNALIGNED_OK_4)
		t += 3;
		if (t >= 8) do
		{
			UA_COPY64(op, ip);
			op += 8; ip += 8; t -= 8;
		} while (t >= 8);
		if (t >= 4)
		{
			UA_COPY32(op, ip);
			op += 4; ip += 4; t -= 4;
		}
		if (t > 0)
		{
			*op++ = *ip++;
			if (t > 1) { *op++ = *ip++; if (t > 2) { *op++ = *ip++; } }
		}
#elif defined(LZO_UNALIGNED_OK_4) || defined(LZO_ALIGNED_OK_4)
#if !defined(LZO_UNALIGNED_OK_4)
		if (PTR_ALIGNED2_4(op, ip))
		{
#endif
			UA_COPY32(op, ip);
			op += 4; ip += 4;
			if (--t > 0)
			{
				if (t >= 4)
				{
					do {
						UA_COPY32(op, ip);
						op += 4; ip += 4; t -= 4;
					} while (t >= 4);
					if (t > 0) do *op++ = *ip++; while (--t > 0);
				}
				else
					do *op++ = *ip++; while (--t > 0);
			}
#if !defined(LZO_UNALIGNED_OK_4)
		}
		else
#endif
#endif
#if !defined(LZO_UNALIGNED_OK_4) && !defined(LZO_UNALIGNED_OK_8)
		{
			*op++ = *ip++; *op++ = *ip++; *op++ = *ip++;
			do *op++ = *ip++; while (--t > 0);
		}
#endif

	first_literal_run:

		t = *ip++;
		if (t >= 16)
			goto match;
#if defined(COPY_DICT)
#if defined(LZO1Z)
		m_off = (1 + M2_MAX_OFFSET) + (t << 6) + (*ip++ >> 2);
		last_m_off = m_off;
#else
		m_off = (1 + M2_MAX_OFFSET) + (t >> 2) + (*ip++ << 2);
#endif
		NEED_OP(3);
		t = 3; COPY_DICT(t, m_off)
#else
#if defined(LZO1Z)
		t = (1 + M2_MAX_OFFSET) + (t << 6) + (*ip++ >> 2);
		m_pos = op - t;
		last_m_off = t;
#else
		m_pos = op - (1 + M2_MAX_OFFSET);
		m_pos -= t >> 2;
		m_pos -= *ip++ << 2;
#endif
		TEST_LB(m_pos); NEED_OP(3);
		*op++ = *m_pos++; *op++ = *m_pos++; *op++ = *m_pos;
#endif
		goto match_done;

		do {
		match:
			if (t >= 64)
			{
#if defined(COPY_DICT)
#if defined(LZO1X)
				m_off = 1 + ((t >> 2) & 7) + (*ip++ << 3);
				t = (t >> 5) - 1;
#elif defined(LZO1Y)
				m_off = 1 + ((t >> 2) & 3) + (*ip++ << 2);
				t = (t >> 4) - 3;
#elif defined(LZO1Z)
				m_off = t & 0x1f;
				if (m_off >= 0x1c)
					m_off = last_m_off;
				else
				{
					m_off = 1 + (m_off << 6) + (*ip++ >> 2);
					last_m_off = m_off;
				}
				t = (t >> 5) - 1;
#endif
#else
#if defined(LZO1X)
				m_pos = op - 1;
				m_pos -= (t >> 2) & 7;
				m_pos -= *ip++ << 3;
				t = (t >> 5) - 1;
#elif defined(LZO1Y)
				m_pos = op - 1;
				m_pos -= (t >> 2) & 3;
				m_pos -= *ip++ << 2;
				t = (t >> 4) - 3;
#elif defined(LZO1Z)
				{
					lzo_uint off = t & 0x1f;
					m_pos = op;
					if (off >= 0x1c)
					{
						assert(last_m_off > 0);
						m_pos -= last_m_off;
					}
					else
					{
						off = 1 + (off << 6) + (*ip++ >> 2);
						m_pos -= off;
						last_m_off = off;
					}
				}
				t = (t >> 5) - 1;
#endif
				TEST_LB(m_pos); assert(t > 0); NEED_OP(t + 3 - 1);
				goto copy_match;
#endif
			}
			else if (t >= 32)
			{
				t &= 31;
				if (t == 0)
				{
					NEED_IP(1);
					while (*ip == 0)
					{
						t += 255;
						ip++;
						NEED_IP(1);
					}
					t += 31 + *ip++;
				}
#if defined(COPY_DICT)
#if defined(LZO1Z)
				m_off = 1 + (ip[0] << 6) + (ip[1] >> 2);
				last_m_off = m_off;
#else
				m_off = 1 + (ip[0] >> 2) + (ip[1] << 6);
#endif
#else
#if defined(LZO1Z)
				{
					lzo_uint off = 1 + (ip[0] << 6) + (ip[1] >> 2);
					m_pos = op - off;
					last_m_off = off;
				}
#elif defined(LZO_UNALIGNED_OK_2) && defined(LZO_ABI_LITTLE_ENDIAN)
				m_pos = op - 1;
				m_pos -= UA_GET16(ip) >> 2;
#else
				m_pos = op - 1;
				m_pos -= (ip[0] >> 2) + (ip[1] << 6);
#endif
#endif
				ip += 2;
			}
			else if (t >= 16)
			{
#if defined(COPY_DICT)
				m_off = (t & 8) << 11;
#else
				m_pos = op;
				m_pos -= (t & 8) << 11;
#endif
				t &= 7;
				if (t == 0)
				{
					NEED_IP(1);
					while (*ip == 0)
					{
						t += 255;
						ip++;
						NEED_IP(1);
					}
					t += 7 + *ip++;
				}
#if defined(COPY_DICT)
#if defined(LZO1Z)
				m_off += (ip[0] << 6) + (ip[1] >> 2);
#else
				m_off += (ip[0] >> 2) + (ip[1] << 6);
#endif
				ip += 2;
				if (m_off == 0)
					goto eof_found;
				m_off += 0x4000;
#if defined(LZO1Z)
				last_m_off = m_off;
#endif
#else
#if defined(LZO1Z)
				m_pos -= (ip[0] << 6) + (ip[1] >> 2);
#elif defined(LZO_UNALIGNED_OK_2) && defined(LZO_ABI_LITTLE_ENDIAN)
				m_pos -= UA_GET16(ip) >> 2;
#else
				m_pos -= (ip[0] >> 2) + (ip[1] << 6);
#endif
				ip += 2;
				if (m_pos == op)
					goto eof_found;
				m_pos -= 0x4000;
#if defined(LZO1Z)
				last_m_off = pd((const lzo_bytep)op, m_pos);
#endif
#endif
			}
			else
			{
#if defined(COPY_DICT)
#if defined(LZO1Z)
				m_off = 1 + (t << 6) + (*ip++ >> 2);
				last_m_off = m_off;
#else
				m_off = 1 + (t >> 2) + (*ip++ << 2);
#endif
				NEED_OP(2);
				t = 2; COPY_DICT(t, m_off)
#else
#if defined(LZO1Z)
				t = 1 + (t << 6) + (*ip++ >> 2);
				m_pos = op - t;
				last_m_off = t;
#else
				m_pos = op - 1;
				m_pos -= t >> 2;
				m_pos -= *ip++ << 2;
#endif
				TEST_LB(m_pos); NEED_OP(2);
				*op++ = *m_pos++; *op++ = *m_pos;
#endif
				goto match_done;
			}

#if defined(COPY_DICT)

			NEED_OP(t + 3 - 1);
			t += 3 - 1; COPY_DICT(t, m_off)

#else

			TEST_LB(m_pos); assert(t > 0); NEED_OP(t + 3 - 1);
#if defined(LZO_UNALIGNED_OK_8) && defined(LZO_UNALIGNED_OK_4)
			if (op - m_pos >= 8)
			{
				t += (3 - 1);
				if (t >= 8) do
				{
					UA_COPY64(op, m_pos);
					op += 8; m_pos += 8; t -= 8;
				} while (t >= 8);
				if (t >= 4)
				{
					UA_COPY32(op, m_pos);
					op += 4; m_pos += 4; t -= 4;
				}
				if (t > 0)
				{
					*op++ = m_pos[0];
					if (t > 1) { *op++ = m_pos[1]; if (t > 2) { *op++ = m_pos[2]; } }
				}
			}
			else
#elif defined(LZO_UNALIGNED_OK_4) || defined(LZO_ALIGNED_OK_4)
#if !defined(LZO_UNALIGNED_OK_4)
			if (t >= 2 * 4 - (3 - 1) && PTR_ALIGNED2_4(op, m_pos))
			{
				assert((op - m_pos) >= 4);
#else
			if (t >= 2 * 4 - (3 - 1) && (op - m_pos) >= 4)
			{
#endif
				UA_COPY32(op, m_pos);
				op += 4; m_pos += 4; t -= 4 - (3 - 1);
				do {
					UA_COPY32(op, m_pos);
					op += 4; m_pos += 4; t -= 4;
				} while (t >= 4);
				if (t > 0) do *op++ = *m_pos++; while (--t > 0);
			}
			else
#endif
			{
			copy_match:
				*op++ = *m_pos++; *op++ = *m_pos++;
				do *op++ = *m_pos++; while (--t > 0);
			}

#endif

		match_done:
#if defined(LZO1Z)
			t = ip[-1] & 3;
#else
			t = ip[-2] & 3;
#endif
			if (t == 0)
				break;

		match_next:
			assert(t > 0); assert(t < 4); NEED_OP(t); NEED_IP(t + 1);
#if 0
			do *op++ = *ip++; while (--t > 0);
#else
			*op++ = *ip++;
			if (t > 1) { *op++ = *ip++; if (t > 2) { *op++ = *ip++; } }
#endif
			t = *ip++;
			} while (TEST_IP && TEST_OP);
		}

#if defined(HAVE_TEST_IP) || defined(HAVE_TEST_OP)
	*out_len = pd(op, out);
	return LZO_E_EOF_NOT_FOUND;
#endif

eof_found:
	assert(t == 1);
	*out_len = pd(op, out);
	return (ip == ip_end ? LZO_E_OK :
		(ip < ip_end ? LZO_E_INPUT_NOT_CONSUMED : LZO_E_INPUT_OVERRUN));

#if defined(HAVE_NEED_IP)
	input_overrun:
				 *out_len = pd(op, out);
				 return LZO_E_INPUT_OVERRUN;
#endif

#if defined(HAVE_NEED_OP)
				 output_overrun:
							   *out_len = pd(op, out);
							   return LZO_E_OUTPUT_OVERRUN;
#endif

#if defined(LZO_TEST_OVERRUN_LOOKBEHIND)
							   lookbehind_overrun:
												 *out_len = pd(op, out);
												 return LZO_E_LOOKBEHIND_OVERRUN;
#endif
	}

#endif
