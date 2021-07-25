#ifndef __MINICVCONFIG_H__
#define __MINICVCONFIG_H__
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdint.h>

#define debug_line printf("[%s %s] %s:%d: %s\n", __DATE__, __TIME__, __FILE__, __LINE__, __func__)

#define FIL FILE

#define NORETURN
#define FATFS const char
#define FA_READ
#define FA_OPEN_EXISTING

#define OMV_FB_ALLOC_SIZE 2048 // minimum fb alloc size
#define OMV_FB_ALLOC_SIZE_END 2047

#define OMV_UMM_BLOCK_SIZE 256

#define JPEG_QUALITY_LOW 50
#define JPEG_QUALITY_HIGH 90

#define OMV_JPEG_BUF_SIZE (1024 * 1024) // IDE JPEG buffer (header + data).
#define JPEG_QUALITY_THRESH (1920 * 1080 * 2)
#define OMV_FB_SIZE (32 * 1024) // FB memory: header + VGA/GS image

#define OMV_RAW_BUF_SIZE (4 * 1024 * 1024)

typedef FILE FIL;
typedef DIR FF_DIR;
typedef char TCHAR;
typedef uint8_t BYTE;
typedef uint32_t UINT;
#define FR_OK 0
typedef int FRESULT; //FR_OK
typedef struct stat FILINFO;

// char _fballoc[OMV_FB_ALLOC_SIZE];

#define DBGLOG_TRACE(format, ...) printf("[%s %s] %s:%d: %s\n", __DATE__, __TIME__, __FILE__, __LINE__, __func__)

#define DBGLOG_DEBUG(format, ...) printf("[%s %s] %s:%d: %s\n", __DATE__, __TIME__, __FILE__, __LINE__, __func__)

#define DBGLOG_ERROR(format, ...) printf("[%s %s] %s:%d: %s\n", __DATE__, __TIME__, __FILE__, __LINE__, __func__)
void __DMB();
void __disable_irq();
void __enable_irq();
void __LDREXW();
void __STREXW();
void __WFI();
void mp_hal_ticks_ms();

// #define __UXTB_RORn(a,b)

// #define IMAGE_LBP_API
// #define IMAGE_ORB_API
#define __STATIC_FORCEINLINE __attribute__((always_inline)) static inline

__STATIC_FORCEINLINE uint32_t __UXTB_RORn(uint32_t op1, uint32_t rotate)
{
	return (op1 >> rotate) & 0xFF;
}

__STATIC_FORCEINLINE uint8_t __CLZ(uint32_t value)
{
	/* Even though __builtin_clz produces a CLZ instruction on ARM, formally
     __builtin_clz(0) is undefined behaviour, so handle this case specially.
     This guarantees ARM-compatible results if happening to compile on a non-ARM
     target, and ensures the compiler doesn't decide to activate any
     optimisations using the logic "value was passed to __builtin_clz, so it
     is non-zero".
     ARM GCC 7.3 and possibly earlier will optimise this test away, leaving a
     single CLZ instruction.
   */
	if (value == 0U)
	{
		return 32U;
	}
	return __builtin_clz(value);
}

__attribute__((always_inline)) static __inline uint32_t __RBIT(uint32_t value)
{
	uint32_t result;
	int32_t s = (4 /*sizeof(v)*/ * 8) - 1; /* extra shift needed at end */

	result = value; /* r will be reversed bits of v; first get LSB of v */
	for (value >>= 1U; value; value >>= 1U)
	{
		result <<= 1U;
		result |= value & 1U;
		s--;
	}
	result <<= s; /* shift when v's highest bits are zero */
	return (result);
}

#define CMSIS_INLINE __attribute__((always_inline))

__attribute__((always_inline)) static __inline int32_t __SSAT(int32_t val, uint32_t sat)
{
	if ((sat >= 1U) && (sat <= 32U))
	{
		const int32_t max = (int32_t)((1U << (sat - 1U)) - 1U);
		const int32_t min = -1 - max;
		if (val > max)
		{
			return max;
		}
		else if (val < min)
		{
			return min;
		}
	}
	return val;
}

typedef int32_t q31_t;

typedef int64_t q63_t;

//   CMSIS_INLINE static __inline q31_t clip_q63_to_q31(
//   q63_t x)
//   {
//     return ((q31_t) (x >> 32) != ((q31_t) x >> 31)) ?
//       ((0x7FFFFFFF ^ ((q31_t) (x >> 63)))) : (q31_t) x;
//   }

CMSIS_INLINE static __inline uint32_t __QADD16(
	uint32_t x,
	uint32_t y)
{
	/*  q31_t r,     s;  without initialisation 'arm_offset_q15 test' fails  but 'intrinsic' tests pass! for armCC */
	q31_t r = 0, s = 0;

	r = __SSAT(((((q31_t)x << 16) >> 16) + (((q31_t)y << 16) >> 16)), 16) & (int32_t)0x0000FFFF;
	s = __SSAT(((((q31_t)x) >> 16) + (((q31_t)y) >> 16)), 16) & (int32_t)0x0000FFFF;

	return ((uint32_t)((s << 16) | (r)));
}

CMSIS_INLINE static __inline uint32_t __SMLAD(
	uint32_t x,
	uint32_t y,
	uint32_t sum)
{
	return ((uint32_t)(((((q31_t)x << 16) >> 16) * (((q31_t)y << 16) >> 16)) +
					   ((((q31_t)x) >> 16) * (((q31_t)y) >> 16)) +
					   (((q31_t)sum))));
}
CMSIS_INLINE static __inline uint32_t __SMUAD(
	uint32_t x,
	uint32_t y)
{
	return ((uint32_t)(((((q31_t)x << 16) >> 16) * (((q31_t)y << 16) >> 16)) +
					   ((((q31_t)x) >> 16) * (((q31_t)y) >> 16))));
}

__STATIC_FORCEINLINE uint32_t __USAT_ASR(int32_t val, uint32_t sat, uint32_t shift)
{
	val >>= shift & 0x1F;

	if (sat <= 31U)
	{
		const uint32_t max = ((1U << sat) - 1U);
		if (val > (int32_t)max)
		{
			return max;
		}
		else if (val < 0)
		{
			return 0U;
		}
	}
	return (uint32_t)val;
}
#define __PKHBT(ARG1, ARG2, ARG3) ((((int32_t)(ARG1) << 0) & (int32_t)0x0000FFFF) | \
								   (((int32_t)(ARG2) << ARG3) & (int32_t)0xFFFF0000))
#define __PKHTB(ARG1, ARG2, ARG3) ((((int32_t)(ARG1) << 0) & (int32_t)0xFFFF0000) | \
								   (((int32_t)(ARG2) >> ARG3) & (int32_t)0x0000FFFF))

__STATIC_FORCEINLINE uint32_t __SSUB16(uint32_t op1, uint32_t op2)
{
	return ((op1 & 0xFFFF0000) - (op2 & 0xFFFF0000)) | ((op1 - op2) & 0xFFFF);
}
__STATIC_FORCEINLINE uint32_t __USAT16(int32_t val, uint32_t sat)
{
	if (sat <= 15U)
	{
		const uint32_t max = ((1U << sat) - 1U);
		int32_t valHi = val >> 16;
		if (valHi > (int32_t)max)
		{
			valHi = max;
		}
		else if (valHi < 0)
		{
			valHi = 0U;
		}
		int32_t valLo = (val << 16) >> 16;
		if (valLo > (int32_t)max)
		{
			valLo = max;
		}
		else if (valLo < 0)
		{
			valLo = 0U;
		}
		return (valHi << 16) | valLo;
	}
	return (uint32_t)val;
}

// #define IMLIB_ENABLE_FIND_LINES 1
// #define IMLIB_ENABLE_FIND_LINE_SEGMENTS 1

// #define IMLIB_ENABLE_GET_REGRESSION 1

// #define IMLIB_ENABLE_FIND_CIRCLES 1
// #define IMLIB_ENABLE_FIND_RECTS 1
// #define IMLIB_ENABLE_QRCODES 1

// #define IMLIB_ENABLE_APRILTAGS 1

// #define IMLIB_ENABLE_APRILTAGS 1

// #define IMLIB_ENABLE_DATAMATRICES 1

// #define IMLIB_ENABLE_BARCODES 1
// #define IMLIB_ENABLE_BINARY_OPS 1

// #define IMLIB_ENABLE_FIND_DISPLACEMENT 1

// #define IMLIB_DRAW_IMAGE 1


uint32_t rng_randint(uint32_t min, uint32_t max);

#endif
