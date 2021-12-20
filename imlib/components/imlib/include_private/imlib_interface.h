#ifndef IMLIB_INTERFACE_H
#define IMLIB_INTERFACE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <math.h>

//POSIX api


//The memory interface
#define xalloc malloc
#define xree free
#define xlloc calloc
#define xemcpy memcpy
#define xemmove memmove
#define xemset memset

//File interface
#define xpen open


#define fast_atanf atanf
#define fast_fabsf fabsf
#define fast_floorf floorf
#define fast_roundf roundf

//cpu is BIG

#define IM_LOG2_2(x)    (((x) &                0x2ULL) ? ( 2                        ) :             1) // NO ({ ... }) !
#define IM_LOG2_4(x)    (((x) &                0xCULL) ? ( 2 +  IM_LOG2_2((x) >>  2)) :  IM_LOG2_2(x)) // NO ({ ... }) !
#define IM_LOG2_8(x)    (((x) &               0xF0ULL) ? ( 4 +  IM_LOG2_4((x) >>  4)) :  IM_LOG2_4(x)) // NO ({ ... }) !
#define IM_LOG2_16(x)   (((x) &             0xFF00ULL) ? ( 8 +  IM_LOG2_8((x) >>  8)) :  IM_LOG2_8(x)) // NO ({ ... }) !
#define IM_LOG2_32(x)   (((x) &         0xFFFF0000ULL) ? (16 + IM_LOG2_16((x) >> 16)) : IM_LOG2_16(x)) // NO ({ ... }) !
#define IM_LOG2(x)      (((x) & 0xFFFFFFFF00000000ULL) ? (32 + IM_LOG2_32((x) >> 32)) : IM_LOG2_32(x)) // NO ({ ... }) !

#define IM_MAX(a,b)     ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define IM_MIN(a,b)     ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define IM_DIV(a,b)     ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _b ? (_a / _b) : 0; })
#define IM_MOD(a,b)     ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _b ? (_a % _b) : 0; })

#define INT8_T_BITS     (sizeof(int8_t) * 8)
#define INT8_T_MASK     (INT8_T_BITS - 1)
#define INT8_T_SHIFT    IM_LOG2(INT8_T_MASK)

#define INT16_T_BITS    (sizeof(int16_t) * 8)
#define INT16_T_MASK    (INT16_T_BITS - 1)
#define INT16_T_SHIFT   IM_LOG2(INT16_T_MASK)

#define INT32_T_BITS    (sizeof(int32_t) * 8)
#define INT32_T_MASK    (INT32_T_BITS - 1)
#define INT32_T_SHIFT   IM_LOG2(INT32_T_MASK)

#define INT64_T_BITS    (sizeof(int64_t) * 8)
#define INT64_T_MASK    (INT64_T_BITS - 1)
#define INT64_T_SHIFT   IM_LOG2(INT64_T_MASK)

#define UINT8_T_BITS    (sizeof(uint8_t) * 8)
#define UINT8_T_MASK    (UINT8_T_BITS - 1)
#define UINT8_T_SHIFT   IM_LOG2(UINT8_T_MASK)

#define UINT16_T_BITS   (sizeof(uint16_t) * 8)
#define UINT16_T_MASK   (UINT16_T_BITS - 1)
#define UINT16_T_SHIFT  IM_LOG2(UINT16_T_MASK)

#define UINT32_T_BITS   (sizeof(uint32_t) * 8)
#define UINT32_T_MASK   (UINT32_T_BITS - 1)
#define UINT32_T_SHIFT  IM_LOG2(UINT32_T_MASK)

// #define UINT64_T_BITS   (sizeof(uint64_t) * 8)
// #define UINT64_T_MASK   (UINT64_T_BITS - 1)
// #define UINT64_T_SHIFT  IM_LOG2(UINT64_T_MASK)

#define IM_DEG2RAD(x)   (((x)*M_PI)/180)
#define IM_RAD2DEG(x)   (((x)*180)/M_PI)

#define PI M_PI


#define IMLIB_ENABLE_YUV_LAB_FUNC

#ifdef IMLIB_ENABLE_YUV_LAB_FUNC

extern int8_t lab_table(uint32_t idx);
extern int8_t yuv_table(uint32_t idx);

#define COLOR_RGB565_TO_L(pixel) lab_table((pixel) * 3)
#define COLOR_RGB565_TO_A(pixel) lab_table(((pixel) * 3) + 1)
#define COLOR_RGB565_TO_B(pixel) lab_table(((pixel) * 3) + 2)
#define COLOR_RGB565_TO_Y(pixel) yuv_table((pixel) * 3)
#define COLOR_RGB565_TO_U(pixel) yuv_table(((pixel) * 3) + 1)
#define COLOR_RGB565_TO_V(pixel) yuv_table(((pixel) * 3) + 2)

// extern int8_t lab_table_b(uint32_t idx);
// extern int8_t yuv_table_b(uint32_t idx);
#define lab_table_b lab_table
#define yuv_table_b yuv_table


#define COLOR_RGB888_TO_L(pixel) lab_table_b((pixel) * 3)
#define COLOR_RGB888_TO_A(pixel) lab_table_b(((pixel) * 3) + 1)
#define COLOR_RGB888_TO_B(pixel) lab_table_b(((pixel) * 3) + 2)
#define COLOR_RGB888_TO_Y(pixel) yuv_table_b((pixel) * 3)
#define COLOR_RGB888_TO_U(pixel) yuv_table_b(((pixel) * 3) + 1)
#define COLOR_RGB888_TO_V(pixel) yuv_table_b(((pixel) * 3) + 2)
#else

// extern const int8_t lab_table[196608];
// extern const int8_t yuv_table[196608];

// #define COLOR_RGB565_TO_L(pixel) lab_table[(pixel) * 3]
// #define COLOR_RGB565_TO_A(pixel) lab_table[((pixel) * 3) + 1]
// #define COLOR_RGB565_TO_B(pixel) lab_table[((pixel) * 3) + 2]
// #define COLOR_RGB565_TO_Y(pixel) yuv_table[(pixel) * 3]
// #define COLOR_RGB565_TO_U(pixel) yuv_table[((pixel) * 3) + 1]
// #define COLOR_RGB565_TO_V(pixel) yuv_table[((pixel) * 3) + 2]


// extern const int8_t lab_table_b[196608];
// extern const int8_t yuv_table_b[196608];

// #define COLOR_RGB888_TO_L(pixel) lab_table_b[(pixel) * 3]
// #define COLOR_RGB888_TO_A(pixel) lab_table_b[((pixel) * 3) + 1]
// #define COLOR_RGB888_TO_B(pixel) lab_table_b[((pixel) * 3) + 2]
// #define COLOR_RGB888_TO_Y(pixel) yuv_table_b[(pixel) * 3]
// #define COLOR_RGB888_TO_U(pixel) yuv_table_b[((pixel) * 3) + 1]
// #define COLOR_RGB888_TO_V(pixel) yuv_table_b[((pixel) * 3) + 2]
#endif
// https://en.wikipedia.org/wiki/Lab_color_space -> CIELAB-CIEXYZ conversions
// https://en.wikipedia.org/wiki/SRGB -> Specification of the transformation



// #define COLOR_THRESHOLD_BINARY(pixel, threshold, invert) \
// ({ \
//     __typeof__ (pixel) _pixel = (pixel); \
//     __typeof__ (threshold) _threshold = (threshold); \
//     __typeof__ (invert) _invert = (invert); \
//     ((_threshold->LMin <= _pixel) && (_pixel <= _threshold->LMax)) ^ _invert; \
// })

// #define COLOR_THRESHOLD_GRAYSCALE(pixel, threshold, invert) \
// ({ \
//     __typeof__ (pixel) _pixel = (pixel); \
//     __typeof__ (threshold) _threshold = (threshold); \
//     __typeof__ (invert) _invert = (invert); \
//     ((_threshold->LMin <= _pixel) && (_pixel <= _threshold->LMax)) ^ _invert; \
// })

// #define COLOR_THRESHOLD_RGB565(pixel, threshold, invert) \
// ({ \
//     __typeof__ (pixel) _pixel = (pixel); \
//     __typeof__ (threshold) _threshold = (threshold); \
//     __typeof__ (invert) _invert = (invert); \
//     uint8_t _l = COLOR_RGB565_TO_L(_pixel); \
//     int8_t _a = COLOR_RGB565_TO_A(_pixel); \
//     int8_t _b = COLOR_RGB565_TO_B(_pixel); \
//     ((_threshold->LMin <= _l) && (_l <= _threshold->LMax) && \
//     (_threshold->AMin <= _a) && (_a <= _threshold->AMax) && \
//     (_threshold->BMin <= _b) && (_b <= _threshold->BMax)) ^ _invert; \
// })

// #define COLOR_THRESHOLD_RGB888(pixel, threshold, invert) \
// ({ \
//     __typeof__ (pixel) _pixel = (pixel); \
//     __typeof__ (threshold) _threshold = (threshold); \
//     __typeof__ (invert) _invert = (invert); \
//     uint8_t _l = COLOR_RGB888_TO_L(_pixel); \
//     int8_t _a = COLOR_RGB888_TO_A(_pixel); \
//     int8_t _b = COLOR_RGB888_TO_B(_pixel); \
//     ((_threshold->LMin <= _l) && (_l <= _threshold->LMax) && \
//     (_threshold->AMin <= _a) && (_a <= _threshold->AMax) && \
//     (_threshold->BMin <= _b) && (_b <= _threshold->BMax)) ^ _invert; \
// })

// #define COLOR_BOUND_BINARY(pixel0, pixel1, threshold) \
// ({ \
//     __typeof__ (pixel0) _pixel0 = (pixel0); \
//     __typeof__ (pixel1) _pixel1 = (pixel1); \
//     __typeof__ (threshold) _threshold = (threshold); \
//     (abs(_pixel0 - _pixel1) <= _threshold); \
// })

// #define COLOR_BOUND_GRAYSCALE(pixel0, pixel1, threshold) \
// ({ \
//     __typeof__ (pixel0) _pixel0 = (pixel0); \
//     __typeof__ (pixel1) _pixel1 = (pixel1); \
//     __typeof__ (threshold) _threshold = (threshold); \
//     (abs(_pixel0 - _pixel1) <= _threshold); \
// })

// #define COLOR_BOUND_RGB565(pixel0, pixel1, threshold) \
// ({ \
//     __typeof__ (pixel0) _pixel0 = (pixel0); \
//     __typeof__ (pixel1) _pixel1 = (pixel1); \
//     __typeof__ (threshold) _threshold = (threshold); \
//     (abs(COLOR_RGB565_TO_R5(_pixel0) - COLOR_RGB565_TO_R5(_pixel1)) <= COLOR_RGB565_TO_R5(_threshold)) && \
//     (abs(COLOR_RGB565_TO_G6(_pixel0) - COLOR_RGB565_TO_G6(_pixel1)) <= COLOR_RGB565_TO_G6(_threshold)) && \
//     (abs(COLOR_RGB565_TO_B5(_pixel0) - COLOR_RGB565_TO_B5(_pixel1)) <= COLOR_RGB565_TO_B5(_threshold)); \
// })

// #define COLOR_BOUND_RGB888(pixel0, pixel1, threshold) \
// ({ \
//     __typeof__ (pixel0) _pixel0 = (pixel0); \
//     __typeof__ (pixel1) _pixel1 = (pixel1); \
//     __typeof__ (threshold) _threshold = (threshold); \
//     (abs(COLOR_RGB888_TO_R8(_pixel0) - COLOR_RGB888_TO_R8(_pixel1)) <= COLOR_RGB888_TO_R8(_threshold)) && \
//     (abs(COLOR_RGB888_TO_G8(_pixel0) - COLOR_RGB888_TO_G8(_pixel1)) <= COLOR_RGB888_TO_G8(_threshold)) && \
//     (abs(COLOR_RGB888_TO_B8(_pixel0) - COLOR_RGB888_TO_B8(_pixel1)) <= COLOR_RGB888_TO_B8(_threshold)); \
// })

#define COLOR_BINARY_MIN 0
#define COLOR_BINARY_MAX 1
#define COLOR_GRAYSCALE_BINARY_MIN 0x00
#define COLOR_GRAYSCALE_BINARY_MAX 0xFF
#define COLOR_RGB565_BINARY_MIN 0x0000
#define COLOR_RGB565_BINARY_MAX 0xFFFF
#define COLOR_RGB888_BINARY_MIN 0x000000
#define COLOR_RGB888_BINARY_MAX 0xFFFFFF

#define COLOR_GRAYSCALE_MIN 0
#define COLOR_GRAYSCALE_MAX 255

#define COLOR_R5_MIN 0
#define COLOR_R5_MAX 31
#define COLOR_G6_MIN 0
#define COLOR_G6_MAX 63
#define COLOR_B5_MIN 0
#define COLOR_B5_MAX 31

#define COLOR_R8_MIN 0
#define COLOR_R8_MAX 255
#define COLOR_G8_MIN 0
#define COLOR_G8_MAX 255
#define COLOR_B8_MIN 0
#define COLOR_B8_MAX 255

#define COLOR_L_MIN 0
#define COLOR_L_MAX 100
#define COLOR_A_MIN -128
#define COLOR_A_MAX 127
#define COLOR_B_MIN -128
#define COLOR_B_MAX 127

#define COLOR_Y_MIN -128
#define COLOR_Y_MAX 127
#define COLOR_U_MIN -128
#define COLOR_U_MAX 127
#define COLOR_V_MIN -128
#define COLOR_V_MAX 127

extern const uint8_t rb528_table[32];
extern const uint8_t g628_table[64];

#define COLOR_R5_TO_R8(color) rb528_table[color]
#define COLOR_G6_TO_G8(color) g628_table[color]
#define COLOR_B5_TO_B8(color) rb528_table[color]

extern const uint8_t rb825_table[256];
extern const uint8_t g826_table[256];

#define COLOR_R8_TO_R5(color) rb825_table[color]
#define COLOR_G8_TO_G6(color) g826_table[color]
#define COLOR_B8_TO_B5(color) rb825_table[color]

// RGB565 Stuff //

#define COLOR_RGB565_TO_R5(pixel) (((pixel) >> 3) & 0x1F)
#define COLOR_RGB565_TO_G6(pixel) \
({ \
    __typeof__ (pixel) _pixel = (pixel); \
    ((_pixel & 0x07) << 3) | (_pixel >> 13); \
})
#define COLOR_RGB565_TO_B5(pixel) (((pixel) >> 8) & 0x1F)
#define COLOR_RGB565_TO_R8(pixel) COLOR_R5_TO_R8(COLOR_RGB565_TO_R5(pixel))
#define COLOR_RGB565_TO_G8(pixel) COLOR_G6_TO_G8(COLOR_RGB565_TO_G6(pixel))
#define COLOR_RGB565_TO_B8(pixel) COLOR_B5_TO_B8(COLOR_RGB565_TO_B5(pixel))

// RGB888 Stuff //
//uint32_t pixel
#define COLOR_RGB888_TO_R8(pixel) \
({ \
    __typeof__ (pixel) _pixel = (pixel); \
    ((_pixel & 0x0000ff00) >> 8)\
})
#define COLOR_RGB888_TO_G8(pixel) \
({ \
    __typeof__ (pixel) _pixel = (pixel); \
    ((_pixel & 0x00ff0000) >> 16)\
})
#define COLOR_RGB888_TO_B8(pixel) \
({ \
    __typeof__ (pixel) _pixel = (pixel); \
    ((_pixel & 0xff000000) >> 24)\
})


// //old OPENMV config
#define COLOR_R5_G6_B5_TO_RGB565(r5, g6, b5) \
({ \
    __typeof__ (r5) _r5 = (r5); \
    __typeof__ (g6) _g6 = (g6); \
    __typeof__ (b5) _b5 = (b5); \
    (_r5 << 3) | (_g6 >> 3) | ((_g6 & 0x7) << 13) | (_b5 << 8); \
})
#define COLOR_R8_G8_B8_TO_RGB888(r8, g8, b8) \
({ \
    __typeof__ (r8) _r8 = (r8); \
    __typeof__ (g8) _g8 = (g8); \
    __typeof__ (b8) _b8 = (b8); \
    (_r8 << 8) | (_g8 << 16)| (_b8 << 24);\
})
//fix MAIX config
/*#define COLOR_R5_G6_B5_TO_RGB565(r5, g6, b5) \
({ \
    __typeof__ (r5) _r5 = (r5); \
    __typeof__ (g6) _g6 = (g6); \
    __typeof__ (b5) _b5 = (b5); \
    ((_r5<<11) | (_g6<<5) | (_b5)); \
})*/

#define COLOR_R8_G8_B8_TO_RGB565(r8, g8, b8) COLOR_R5_G6_B5_TO_RGB565(COLOR_R8_TO_R5(r8), COLOR_G8_TO_G6(g8), COLOR_B8_TO_B5(b8))



#define COLOR_LAB_TO_RGB565(l, a, b) \
({ \
    __typeof__ (l) _l = (l); \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    float _x = ((_l + 16) * 0.008621f) + (_a * 0.002f); \
    float _y = ((_l + 16) * 0.008621f); \
    float _z = ((_l + 16) * 0.008621f) - (_b * 0.005f); \
    _x = ((_x > 0.206897f) ? (_x * _x * _x) : ((0.128419f * _x) - 0.017713f)) * 095.047f; \
    _y = ((_y > 0.206897f) ? (_y * _y * _y) : ((0.128419f * _y) - 0.017713f)) * 100.000f; \
    _z = ((_z > 0.206897f) ? (_z * _z * _z) : ((0.128419f * _z) - 0.017713f)) * 108.883f; \
    float _r_lin = ((_x * +3.2406f) + (_y * -1.5372f) + (_z * -0.4986f)) / 100.0f; \
    float _g_lin = ((_x * -0.9689f) + (_y * +1.8758f) + (_z * +0.0415f)) / 100.0f; \
    float _b_lin = ((_x * +0.0557f) + (_y * -0.2040f) + (_z * +1.0570f)) / 100.0f; \
    _r_lin = (_r_lin > 0.0031308f) ? ((1.055f * powf(_r_lin, 0.416666f)) - 0.055f) : (_r_lin * 12.92f); \
    _g_lin = (_g_lin > 0.0031308f) ? ((1.055f * powf(_g_lin, 0.416666f)) - 0.055f) : (_g_lin * 12.92f); \
    _b_lin = (_b_lin > 0.0031308f) ? ((1.055f * powf(_b_lin, 0.416666f)) - 0.055f) : (_b_lin * 12.92f); \
    unsigned int _rgb565_r = IM_MAX(IM_MIN(roundf(_r_lin * COLOR_R5_MAX), COLOR_R5_MAX), COLOR_R5_MIN); \
    unsigned int _rgb565_g = IM_MAX(IM_MIN(roundf(_g_lin * COLOR_G6_MAX), COLOR_G6_MAX), COLOR_G6_MIN); \
    unsigned int _rgb565_b = IM_MAX(IM_MIN(roundf(_b_lin * COLOR_B5_MAX), COLOR_B5_MAX), COLOR_B5_MIN); \
    COLOR_R5_G6_B5_TO_RGB565(_rgb565_r, _rgb565_g, _rgb565_b); \
})

#define COLOR_LAB_TO_RGB888(l, a, b) \
({ \
    __typeof__ (l) _l = (l); \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    float _x = ((_l + 16) * 0.008621f) + (_a * 0.002f); \
    float _y = ((_l + 16) * 0.008621f); \
    float _z = ((_l + 16) * 0.008621f) - (_b * 0.005f); \
    _x = ((_x > 0.206897f) ? (_x * _x * _x) : ((0.128419f * _x) - 0.017713f)) * 095.047f; \
    _y = ((_y > 0.206897f) ? (_y * _y * _y) : ((0.128419f * _y) - 0.017713f)) * 100.000f; \
    _z = ((_z > 0.206897f) ? (_z * _z * _z) : ((0.128419f * _z) - 0.017713f)) * 108.883f; \
    float _r_lin = ((_x * +3.2406f) + (_y * -1.5372f) + (_z * -0.4986f)) / 100.0f; \
    float _g_lin = ((_x * -0.9689f) + (_y * +1.8758f) + (_z * +0.0415f)) / 100.0f; \
    float _b_lin = ((_x * +0.0557f) + (_y * -0.2040f) + (_z * +1.0570f)) / 100.0f; \
    _r_lin = (_r_lin > 0.0031308f) ? ((1.055f * powf(_r_lin, 0.416666f)) - 0.055f) : (_r_lin * 12.92f); \
    _g_lin = (_g_lin > 0.0031308f) ? ((1.055f * powf(_g_lin, 0.416666f)) - 0.055f) : (_g_lin * 12.92f); \
    _b_lin = (_b_lin > 0.0031308f) ? ((1.055f * powf(_b_lin, 0.416666f)) - 0.055f) : (_b_lin * 12.92f); \
    unsigned int _rgb888_r = IM_MAX(IM_MIN(roundf(_r_lin * COLOR_R8_MAX), COLOR_R8_MAX), COLOR_R8_MIN); \
    unsigned int _rgb888_g = IM_MAX(IM_MIN(roundf(_g_lin * COLOR_G8_MAX), COLOR_G8_MAX), COLOR_G8_MIN); \
    unsigned int _rgb888_b = IM_MAX(IM_MIN(roundf(_b_lin * COLOR_B8_MAX), COLOR_B8_MAX), COLOR_B8_MIN); \
    COLOR_R8_G8_B8_TO_RGB888(_rgb888_r, _rgb888_g, _rgb888_b); \
})
// // https://en.wikipedia.org/wiki/YCbCr -> JPEG Conversion

#define COLOR_YUV_TO_RGB565(y, u, v) \
({ \
    __typeof__ (y) _y = (y); \
    __typeof__ (u) _u = (u); \
    __typeof__ (v) _v = (v); \
    unsigned int _r = IM_MAX(IM_MIN(128 + _y + ((((uint32_t) ((1.402000 * 65536) + 0.5)) * _v) >> 16), COLOR_R8_MAX), COLOR_R8_MIN); \
    unsigned int _g = IM_MAX(IM_MIN(128 + _y - (((((uint32_t) ((0.344136 * 65536) + 0.5)) * _u) + (((uint32_t) ((0.714136 * 65536) + 0.5)) * _v)) >> 16), COLOR_G8_MAX), COLOR_G8_MIN); \
    unsigned int _b = IM_MAX(IM_MIN(128 + _y + ((((uint32_t) ((1.772000 * 65536) + 0.5)) * _u) >> 16), COLOR_B8_MAX), COLOR_B8_MIN); \
    COLOR_R8_G8_B8_TO_RGB565(_r, _g, _b); \
})

#define COLOR_YUV_TO_RGB888(y, u, v) \
({ \
    __typeof__ (y) _y = (y); \
    __typeof__ (u) _u = (u); \
    __typeof__ (v) _v = (v); \
    unsigned int _r = IM_MAX(IM_MIN(128 + _y + ((((uint32_t) ((1.402000 * 65536) + 0.5)) * _v) >> 16), COLOR_R8_MAX), COLOR_R8_MIN); \
    unsigned int _g = IM_MAX(IM_MIN(128 + _y - (((((uint32_t) ((0.344136 * 65536) + 0.5)) * _u) + (((uint32_t) ((0.714136 * 65536) + 0.5)) * _v)) >> 16), COLOR_G8_MAX), COLOR_G8_MIN); \
    unsigned int _b = IM_MAX(IM_MIN(128 + _y + ((((uint32_t) ((1.772000 * 65536) + 0.5)) * _u) >> 16), COLOR_B8_MAX), COLOR_B8_MIN); \
    COLOR_R8_G8_B8_TO_RGB888(_r, _g, _b); \
})

#define COLOR_BAYER_TO_RGB565(img, x, y, r, g, b)            \
({                                                           \
    __typeof__ (x) __x = (x);                                \
    __typeof__ (y) __y = (y);                                \
    if ((__y % 2) == 0) {                                    \
        if ((__x % 2) == 0) {                                \
            r = (IM_GET_RAW_PIXEL(img, __x-1, __y-1)  +      \
                 IM_GET_RAW_PIXEL(img, __x+1, __y-1)  +      \
                 IM_GET_RAW_PIXEL(img, __x-1, __y+1)  +      \
                 IM_GET_RAW_PIXEL(img, __x+1, __y+1)) >> 2;  \
                                                             \
            g = (IM_GET_RAW_PIXEL(img, __x,   __y-1)  +      \
                 IM_GET_RAW_PIXEL(img, __x,   __y+1)  +      \
                 IM_GET_RAW_PIXEL(img, __x-1, __y)    +      \
                 IM_GET_RAW_PIXEL(img, __x+1, __y))   >> 2;  \
                                                             \
            b = IM_GET_RAW_PIXEL(img,  __x, __y);            \
        } else {                                             \
            r = (IM_GET_RAW_PIXEL(img, __x, __y-1)  +        \
                 IM_GET_RAW_PIXEL(img, __x, __y+1)) >> 1;    \
                                                             \
            b = (IM_GET_RAW_PIXEL(img, __x-1, __y)  +        \
                 IM_GET_RAW_PIXEL(img, __x+1, __y)) >> 1;    \
                                                             \
            g =  IM_GET_RAW_PIXEL(img, __x, __y);            \
        }                                                    \
    } else {                                                 \
        if ((__x % 2) == 0) {                                \
            r = (IM_GET_RAW_PIXEL(img, __x-1, __y)  +        \
                 IM_GET_RAW_PIXEL(img, __x+1, __y)) >> 1;    \
                                                             \
            g =  IM_GET_RAW_PIXEL(img, __x, __y);            \
                                                             \
            b = (IM_GET_RAW_PIXEL(img, __x, __y-1)  +        \
                 IM_GET_RAW_PIXEL(img, __x, __y+1)) >> 1;    \
        } else {                                             \
            r = IM_GET_RAW_PIXEL(img,  __x, __y);            \
                                                             \
            g = (IM_GET_RAW_PIXEL(img, __x, __y-1)    +      \
                 IM_GET_RAW_PIXEL(img, __x, __y+1)    +      \
                 IM_GET_RAW_PIXEL(img, __x-1, __y)    +      \
                 IM_GET_RAW_PIXEL(img, __x+1, __y))   >> 2;  \
                                                             \
            b = (IM_GET_RAW_PIXEL(img, __x-1, __y-1)  +      \
                 IM_GET_RAW_PIXEL(img, __x+1, __y-1)  +      \
                 IM_GET_RAW_PIXEL(img, __x-1, __y+1)  +      \
                 IM_GET_RAW_PIXEL(img, __x+1, __y+1)) >> 2;  \
        }                                                    \
    }                                                        \
    r  = IM_R825(r);                                         \
    g  = IM_G826(g);                                         \
    b  = IM_B825(b);                                         \
})

#define COLOR_BAYER_TO_RGB565(img, x, y, r, g, b)            \
({                                                           \
    __typeof__ (x) __x = (x);                                \
    __typeof__ (y) __y = (y);                                \
    if ((__y % 2) == 0) {                                    \
        if ((__x % 2) == 0) {                                \
            r = (IM_GET_RAW_PIXEL(img, __x-1, __y-1)  +      \
                 IM_GET_RAW_PIXEL(img, __x+1, __y-1)  +      \
                 IM_GET_RAW_PIXEL(img, __x-1, __y+1)  +      \
                 IM_GET_RAW_PIXEL(img, __x+1, __y+1)) >> 2;  \
                                                             \
            g = (IM_GET_RAW_PIXEL(img, __x,   __y-1)  +      \
                 IM_GET_RAW_PIXEL(img, __x,   __y+1)  +      \
                 IM_GET_RAW_PIXEL(img, __x-1, __y)    +      \
                 IM_GET_RAW_PIXEL(img, __x+1, __y))   >> 2;  \
                                                             \
            b = IM_GET_RAW_PIXEL(img,  __x, __y);            \
        } else {                                             \
            r = (IM_GET_RAW_PIXEL(img, __x, __y-1)  +        \
                 IM_GET_RAW_PIXEL(img, __x, __y+1)) >> 1;    \
                                                             \
            b = (IM_GET_RAW_PIXEL(img, __x-1, __y)  +        \
                 IM_GET_RAW_PIXEL(img, __x+1, __y)) >> 1;    \
                                                             \
            g =  IM_GET_RAW_PIXEL(img, __x, __y);            \
        }                                                    \
    } else {                                                 \
        if ((__x % 2) == 0) {                                \
            r = (IM_GET_RAW_PIXEL(img, __x-1, __y)  +        \
                 IM_GET_RAW_PIXEL(img, __x+1, __y)) >> 1;    \
                                                             \
            g =  IM_GET_RAW_PIXEL(img, __x, __y);            \
                                                             \
            b = (IM_GET_RAW_PIXEL(img, __x, __y-1)  +        \
                 IM_GET_RAW_PIXEL(img, __x, __y+1)) >> 1;    \
        } else {                                             \
            r = IM_GET_RAW_PIXEL(img,  __x, __y);            \
                                                             \
            g = (IM_GET_RAW_PIXEL(img, __x, __y-1)    +      \
                 IM_GET_RAW_PIXEL(img, __x, __y+1)    +      \
                 IM_GET_RAW_PIXEL(img, __x-1, __y)    +      \
                 IM_GET_RAW_PIXEL(img, __x+1, __y))   >> 2;  \
                                                             \
            b = (IM_GET_RAW_PIXEL(img, __x-1, __y-1)  +      \
                 IM_GET_RAW_PIXEL(img, __x+1, __y-1)  +      \
                 IM_GET_RAW_PIXEL(img, __x-1, __y+1)  +      \
                 IM_GET_RAW_PIXEL(img, __x+1, __y+1)) >> 2;  \
        }                                                    \
    }                                                        \
    // r  = IM_R825(r);                                         \
    // g  = IM_G826(g);                                         \
    // b  = IM_B825(b);                                         \
})

#define COLOR_BINARY_TO_GRAYSCALE(pixel) ((pixel) * COLOR_GRAYSCALE_MAX)
#define COLOR_BINARY_TO_RGB565(pixel) COLOR_YUV_TO_RGB565((pixel) ? 127 : -128, 0, 0)
#define COLOR_BINARY_TO_RGB888(pixel) COLOR_YUV_TO_RGB888((pixel) ? 127 : -128, 0, 0)
#define COLOR_RGB565_TO_BINARY(pixel) (COLOR_RGB565_TO_Y(pixel) > (((COLOR_Y_MAX - COLOR_Y_MIN) / 2) + COLOR_Y_MIN))
#define COLOR_RGB888_TO_BINARY(pixel) (COLOR_RGB888_TO_Y(pixel) > (((COLOR_Y_MAX - COLOR_Y_MIN) / 2) + COLOR_Y_MIN))
#define COLOR_RGB565_TO_GRAYSCALE(pixel) (COLOR_RGB565_TO_Y(pixel) + 128)
#define COLOR_RGB888_TO_GRAYSCALE(pixel) (COLOR_RGB888_TO_Y(pixel) + 128)
#define COLOR_GRAYSCALE_TO_BINARY(pixel) ((pixel) > (((COLOR_GRAYSCALE_MAX - COLOR_GRAYSCALE_MIN) / 2) + COLOR_GRAYSCALE_MIN))
#define COLOR_GRAYSCALE_TO_RGB565(pixel) COLOR_YUV_TO_RGB565((pixel) - 128, 0, 0)
#define COLOR_GRAYSCALE_TO_RGB888(pixel) COLOR_YUV_TO_RGB888((pixel) - 128, 0, 0)

#define COLOR_RGB565_TO_RGB888(pixel)


// #define IMAGE_IS_MUTABLE(image) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     (_image->bpp == IMAGE_BPP_BINARY) || \
//     (_image->bpp == IMAGE_BPP_GRAYSCALE) || \
//     (_image->bpp == IMAGE_BPP_RGB888) || \
//     (_image->bpp == IMAGE_BPP_RGBA8888) || \
//     (_image->bpp == IMAGE_BPP_RGB565); \
// })

// #define IMAGE_IS_MUTABLE_BAYER(image) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     (_image->bpp == IMAGE_BPP_BINARY) || \
//     (_image->bpp == IMAGE_BPP_GRAYSCALE) || \
//     (_image->bpp == IMAGE_BPP_RGB565) || \
//     (_image->bpp == IMAGE_BPP_RGB888) || \
//     (_image->bpp == IMAGE_BPP_RGBA8888) || \
//     (_image->bpp == IMAGE_BPP_BAYER); \
// })

// #define IMAGE_BINARY_LINE_LEN(image) (((image)->w + UINT32_T_MASK) >> UINT32_T_SHIFT)
// #define IMAGE_BINARY_LINE_LEN_BYTES(image) (IMAGE_BINARY_LINE_LEN(image) * sizeof(uint32_t))

// #define IMAGE_GRAYSCALE_LINE_LEN(image) ((image)->w)
// #define IMAGE_GRAYSCALE_LINE_LEN_BYTES(image) (IMAGE_GRAYSCALE_LINE_LEN(image) * sizeof(uint8_t))

// #define IMAGE_RGB565_LINE_LEN(image) ((image)->w)
// #define IMAGE_RGB565_LINE_LEN_BYTES(image) (IMAGE_RGB565_LINE_LEN(image) * sizeof(uint16_t))

// #define IMAGE_RGB888_LINE_LEN(image) ((image)->w)
// #define IMAGE_RGB888_LINE_LEN_BYTES(image) (IMAGE_RGB888_LINE_LEN(image) * 3)



#define IMAGE_GET_BINARY_PIXEL(image, x, y) \
({ \
    __typeof__ (image) _image = (image); \
    __typeof__ (x) _x = (x); \
    __typeof__ (y) _y = (y); \
    (((uint32_t *) _image->data)[(((_image->w + UINT32_T_MASK) >> UINT32_T_SHIFT) * _y) + (_x >> UINT32_T_SHIFT)] >> (_x & UINT32_T_MASK)) & 1; \
})

#define IMAGE_PUT_BINARY_PIXEL(image, x, y, v) \
({ \
    __typeof__ (image) _image = (image); \
    __typeof__ (x) _x = (x); \
    __typeof__ (y) _y = (y); \
    __typeof__ (v) _v = (v); \
    size_t _i = (((_image->w + UINT32_T_MASK) >> UINT32_T_SHIFT) * _y) + (_x >> UINT32_T_SHIFT); \
    size_t _j = _x & UINT32_T_MASK; \
    ((uint32_t *) _image->data)[_i] = (((uint32_t *) _image->data)[_i] & (~(1 << _j))) | ((_v & 1) << _j); \
})

// #define IMAGE_CLEAR_BINARY_PIXEL(image, x, y) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     __typeof__ (x) _x = (x); \
//     __typeof__ (y) _y = (y); \
//     ((uint32_t *) _image->data)[(((_image->w + UINT32_T_MASK) >> UINT32_T_SHIFT) * _y) + (_x >> UINT32_T_SHIFT)] &= ~(1 << (_x & UINT32_T_MASK)); \
// })

// #define IMAGE_SET_BINARY_PIXEL(image, x, y) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     __typeof__ (x) _x = (x); \
//     __typeof__ (y) _y = (y); \
//     ((uint32_t *) _image->data)[(((_image->w + UINT32_T_MASK) >> UINT32_T_SHIFT) * _y) + (_x >> UINT32_T_SHIFT)] |= 1 << (_x & UINT32_T_MASK); \
// })

#define IMAGE_GET_GRAYSCALE_PIXEL(image, x, y) \
({ \
    __typeof__ (image) _image = (image); \
    __typeof__ (x) _x = (x); \
    __typeof__ (y) _y = (y); \
    ((uint8_t *) _image->data)[(_image->w * _y) + _x]; \
})

#define IMAGE_PUT_GRAYSCALE_PIXEL(image, x, y, v) \
({ \
    __typeof__ (image) _image = (image); \
    __typeof__ (x) _x = (x); \
    __typeof__ (y) _y = (y); \
    __typeof__ (v) _v = (v); \
    ((uint8_t *) _image->data)[(_image->w * _y) + _x] = _v; \
})

#define IMAGE_GET_RGB565_PIXEL(image, x, y) \
({ \
    __typeof__ (image) _image = (image); \
    __typeof__ (x) _x = (x); \
    __typeof__ (y) _y = (y); \
    ((uint16_t *) _image->data)[(_image->w * _y) + _x]; \
})

#define IMAGE_GET_RGB888_PIXEL(image, x, y) \
({ \
    __typeof__ (image) _image = (image); \
    __typeof__ (x) _x = (x); \
    __typeof__ (y) _y = (y); \
    *((uint32_t*)(_image->data + _image->w * _y * 3 + _x * 3));\
})

#define IMAGE_PUT_RGB565_PIXEL(image, x, y, v) \
({ \
    __typeof__ (image) _image = (image); \
    __typeof__ (x) _x = (x); \
    __typeof__ (y) _y = (y); \
    __typeof__ (v) _v = (v); \
    ((uint16_t *) _image->data)[(_image->w * _y) + _x] = _v; \
})

#define IMAGE_PUT_RGB888_PIXEL(image, x, y, v) \
({ \
    __typeof__ (image) _image = (image); \
    __typeof__ (x) _x = (x); \
    __typeof__ (y) _y = (y); \
    __typeof__ (v) _v = (v); \
    *((uint32_t*)(_image->data + _image->w * _y * 3 + _x * 3)) = _v; \
})

// #ifdef __arm__
//     #define IMAGE_REV_RGB565_PIXEL(pixel) \
//     ({ \
//         __typeof__ (pixel) _pixel = (pixel); \
//         __REV16(_pixel); \
//     })

// #else
//     #define IMAGE_REV_RGB565_PIXEL(pixel) \
//     ({ \
//         __typeof__ (pixel) _pixel = (pixel); \
//         ((_pixel >> 8) | (_pixel << 8)) & 0xFFFF; \
//     })
// #endif
// #define IMAGE_REV_RGB888_PIXEL(pixel) \
// ({ \
//     __typeof__ (pixel) _pixel = (pixel); \
//     ((_pixel >> 24) | ((_pixel & 0x00ff0000) >> 16) | ((_pixel & 0x0000ff00) >> 8) | (_pixel & 0x000000ff)) & 0xffffffff;\
// })
// #define IMAGE_COMPUTE_TARGET_SIZE_SCALE_FACTOR(target_size, source_rect) \
// __typeof__ (target_size) _target_size = (target_size); \
// __typeof__ (source_rect) _source_rect = (source_rect); \
// int IMAGE_X_SOURCE_OFFSET = _source_rect->p.x; \
// int IMAGE_Y_SOURCE_OFFSET = _source_rect->p.y; \
// int IMAGE_X_TARGET_OFFSET = 0; \
// int IMAGE_Y_TARGET_OFFSET = 0; \
// float IMAGE_X_RATIO = ((float) _source_rect->s.w) / ((float) _target_size->w); \
// float IMAGE_Y_RATIO = ((float) _source_rect->s.h) / ((float) _target_size->h); \
// ({ 0; })

// #define IMAGE_COMPUTE_TARGET_RECT_SCALE_FACTOR(target_rect, source_rect) \
// __typeof__ (target_rect) _target_rect = (target_rect); \
// __typeof__ (source_rect) _source_rect = (source_rect); \
// int IMAGE_X_SOURCE_OFFSET = _source_rect->p.x; \
// int IMAGE_Y_SOURCE_OFFSET = _source_rect->p.y; \
// int IMAGE_X_TARGET_OFFSET = _target_rect->p.x; \
// int IMAGE_Y_TARGET_OFFSET = _target_rect->p.y; \
// float IMAGE_X_RATIO = ((float) _source_rect->s.w) / ((float) _target_rect->s.w); \
// float IMAGE_Y_RATIO = ((float) _source_rect->s.h) / ((float) _target_rect->s.h); \
// ({ 0; })

// #define IMAGE_GET_SCALED_BINARY_PIXEL(image, x, y) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     __typeof__ (x) _x = (x); \
//     __typeof__ (y) _y = (y); \
//     IMAGE_GET_BINARY_PIXEL(_image, ((size_t) ((IMAGE_X_RATIO * (_x - IMAGE_X_TARGET_OFFSET)) + 0.5)) + IMAGE_X_SOURCE_OFFSET, ((size_t) ((IMAGE_Y_RATIO * (_y - IMAGE_Y_TARGET_OFFSET)) + 0.5)) + IMAGE_Y_SOURCE_OFFSET); \
// })

// #define IMAGE_GET_SCALED_GRAYSCALE_PIXEL(image, x, y) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     __typeof__ (x) _x = (x); \
//     __typeof__ (y) _y = (y); \
//     IMAGE_GET_GRAYSCALE_PIXEL(_image, ((size_t) ((IMAGE_X_RATIO * (_x - IMAGE_X_TARGET_OFFSET)) + 0.5)) + IMAGE_X_SOURCE_OFFSET, ((size_t) ((IMAGE_Y_RATIO * (_y - IMAGE_Y_TARGET_OFFSET)) + 0.5)) + IMAGE_Y_SOURCE_OFFSET); \
// })

// #define IMAGE_GET_SCALED_RGB565_PIXEL(image, x, y) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     __typeof__ (x) _x = (x); \
//     __typeof__ (y) _y = (y); \
//     IMAGE_GET_RGB565_PIXEL(_image, ((size_t) ((IMAGE_X_RATIO * (_x - IMAGE_X_TARGET_OFFSET)) + 0.5)) + IMAGE_X_SOURCE_OFFSET, ((size_t) ((IMAGE_Y_RATIO * (_y - IMAGE_Y_TARGET_OFFSET)) + 0.5)) + IMAGE_Y_SOURCE_OFFSET); \
// })

// #define IMAGE_GET_SCALED_RGB888_PIXEL(image, x, y) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     __typeof__ (x) _x = (x); \
//     __typeof__ (y) _y = (y); \
//     IMAGE_GET_RGB888_PIXEL(_image, ((size_t) ((IMAGE_X_RATIO * (_x - IMAGE_X_TARGET_OFFSET)) + 0.5)) + IMAGE_X_SOURCE_OFFSET, ((size_t) ((IMAGE_Y_RATIO * (_y - IMAGE_Y_TARGET_OFFSET)) + 0.5)) + IMAGE_Y_SOURCE_OFFSET); \
// })
// // Fast Stuff //

// #define IMAGE_COMPUTE_BINARY_PIXEL_ROW_PTR(image, y) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     __typeof__ (y) _y = (y); \
//     ((uint32_t *) _image->data) + (((_image->w + UINT32_T_MASK) >> UINT32_T_SHIFT) * _y); \
// })

// #define IMAGE_INC_BINARY_PIXEL_ROW_PTR(row_ptr, image) \
// ({ \
//     __typeof__ (row_ptr) _row_ptr = (row_ptr); \
//     __typeof__ (image) _image = (image); \
//     _row_ptr + ((_image->w + UINT32_T_MASK) >> UINT32_T_SHIFT); \
// })

// #define IMAGE_GET_BINARY_PIXEL_FAST(row_ptr, x) \
// ({ \
//     __typeof__ (row_ptr) _row_ptr = (row_ptr); \
//     __typeof__ (x) _x = (x); \
//     (_row_ptr[_x >> UINT32_T_SHIFT] >> (_x & UINT32_T_MASK)) & 1; \
// })

// #define IMAGE_PUT_BINARY_PIXEL_FAST(row_ptr, x, v) \
// ({ \
//     __typeof__ (row_ptr) _row_ptr = (row_ptr); \
//     __typeof__ (x) _x = (x); \
//     __typeof__ (v) _v = (v); \
//     size_t _i = _x >> UINT32_T_SHIFT; \
//     size_t _j = _x & UINT32_T_MASK; \
//     _row_ptr[_i] = (_row_ptr[_i] & (~(1 << _j))) | ((_v & 1) << _j); \
// })

// #define IMAGE_CLEAR_BINARY_PIXEL_FAST(row_ptr, x) \
// ({ \
//     __typeof__ (row_ptr) _row_ptr = (row_ptr); \
//     __typeof__ (x) _x = (x); \
//     _row_ptr[_x >> UINT32_T_SHIFT] &= ~(1 << (_x & UINT32_T_MASK)); \
// })

// #define IMAGE_SET_BINARY_PIXEL_FAST(row_ptr, x) \
// ({ \
//     __typeof__ (row_ptr) _row_ptr = (row_ptr); \
//     __typeof__ (x) _x = (x); \
//     _row_ptr[_x >> UINT32_T_SHIFT] |= 1 << (_x & UINT32_T_MASK); \
// })

// #define IMAGE_COMPUTE_GRAYSCALE_PIXEL_ROW_PTR(image, y) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     __typeof__ (y) _y = (y); \
//     ((uint8_t *) _image->data) + (_image->w * _y); \
// })

// #define IMAGE_INC_GRAYSCALE_PIXEL_ROW_PTR(row_ptr, image) \
// ({ \
//     __typeof__ (row_ptr) _row_ptr = (row_ptr); \
//     __typeof__ (image) _image = (image); \
//     row_ptr + _image->w; \
// })

#define IMAGE_GET_GRAYSCALE_PIXEL_FAST(row_ptr, x) \
({ \
    __typeof__ (row_ptr) _row_ptr = (row_ptr); \
    __typeof__ (x) _x = (x); \
    _row_ptr[_x]; \
})

// #define IMAGE_PUT_GRAYSCALE_PIXEL_FAST(row_ptr, x, v) \
// ({ \
//     __typeof__ (row_ptr) _row_ptr = (row_ptr); \
//     __typeof__ (x) _x = (x); \
//     __typeof__ (v) _v = (v); \
//     _row_ptr[_x] = _v; \
// })

// #define IMAGE_COMPUTE_RGB565_PIXEL_ROW_PTR(image, y) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     __typeof__ (y) _y = (y); \
//     ((uint16_t *) _image->data) + (_image->w * _y); \
// })

// #define IMAGE_COMPUTE_RGB888_PIXEL_ROW_PTR(image, y) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     __typeof__ (y) _y = (y); \
//     void *_row_ptr = _image->data + _image->w * _y * 3;\
//     (uint32_t *)_row_ptr;\
// })

// #define IMAGE_INC_RGB565_PIXEL_ROW_PTR(row_ptr, image) \
// ({ \
//     __typeof__ (row_ptr) _row_ptr = (row_ptr); \
//     __typeof__ (image) _image = (image); \
//     row_ptr + _image->w; \
// })

// #define IMAGE_INC_RGB888_PIXEL_ROW_PTR(row_ptr, image) \
// ({ \
//     __typeof__ (row_ptr) _row_ptr = (row_ptr); \
//     __typeof__ (image) _image = (image); \
//     row_ptr + _image->w * 3; \
// })

// #define IMAGE_GET_RGB565_PIXEL_FAST(row_ptr, x) \
// ({ \
//     __typeof__ (row_ptr) _row_ptr = (row_ptr); \
//     __typeof__ (x) _x = (x); \
//     _row_ptr[_x]; \
// })

// #define IMAGE_GET_RGB888_PIXEL_FAST(row_ptr, x) \
// ({ \
//     __typeof__ (row_ptr) _row_ptr = (row_ptr); \
//     __typeof__ (x) _x = (x); \
//     _row_ptr[_x]; \
// })

// #define IMAGE_PUT_RGB565_PIXEL_FAST(row_ptr, x, v) \
// ({ \
//     __typeof__ (row_ptr) _row_ptr = (row_ptr); \
//     __typeof__ (x) _x = (x); \
//     __typeof__ (v) _v = (v); \
//     _row_ptr[_x] = _v; \
// })

// #define IMAGE_PUT_RGB888_PIXEL_FAST(row_ptr, x, v) \
// ({ \
//     __typeof__ (row_ptr) _row_ptr = (row_ptr); \
//     __typeof__ (x) _x = (x); \
//     __typeof__ (v) _v = (v); \
//     _row_ptr[_x] = _v; \
// })

// #define IMAGE_COMPUTE_SCALED_BINARY_PIXEL_ROW_PTR(image, y) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     __typeof__ (y) _y = (y); \
//     ((uint32_t *) _image->data) + (((_image->w + UINT32_T_MASK) >> UINT32_T_SHIFT) * (((size_t) ((IMAGE_Y_RATIO * (_y - IMAGE_Y_TARGET_OFFSET)) + 0.5)) + IMAGE_Y_SOURCE_OFFSET)); \
// })

// #define IMAGE_GET_SCALED_BINARY_PIXEL_FAST(row_ptr, x) IMAGE_GET_BINARY_PIXEL_FAST((row_ptr), ((size_t) ((IMAGE_X_RATIO * ((x) - IMAGE_X_TARGET_OFFSET)) + 0.5)) + IMAGE_X_SOURCE_OFFSET)

// #define IMAGE_COMPUTE_SCALED_GRAYSCALE_PIXEL_ROW_PTR(image, y) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     __typeof__ (y) _y = (y); \
//     ((uint8_t *) _image->data) + (_image->w * (((size_t) ((IMAGE_Y_RATIO * (_y - IMAGE_Y_TARGET_OFFSET)) + 0.5)) + IMAGE_Y_SOURCE_OFFSET)); \
// })

// #define IMAGE_GET_SCALED_GRAYSCALE_PIXEL_FAST(row_ptr, x) IMAGE_GET_GRAYSCALE_PIXEL_FAST((row_ptr), ((size_t) ((IMAGE_X_RATIO * ((x) - IMAGE_X_TARGET_OFFSET)) + 0.5)) + IMAGE_X_SOURCE_OFFSET)

// #define IMAGE_COMPUTE_SCALED_RGB565_PIXEL_ROW_PTR(image, y) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     __typeof__ (y) _y = (y); \
//     ((uint16_t *) _image->data) + (_image->w * (((size_t) ((IMAGE_Y_RATIO * (_y - IMAGE_Y_TARGET_OFFSET)) + 0.5)) + IMAGE_Y_SOURCE_OFFSET)); \
// })

// #define IMAGE_COMPUTE_SCALED_RGB888_PIXEL_ROW_PTR(image, y) \
// ({ \
//     __typeof__ (image) _image = (image); \
//     __typeof__ (y) _y = (y); \
//     ((uint16_t *) _image->data) + (_image->w * (((size_t) ((IMAGE_Y_RATIO * (_y - IMAGE_Y_TARGET_OFFSET)) + 0.5)) + IMAGE_Y_SOURCE_OFFSET)); \
// })

// #define IMAGE_GET_SCALED_RGB565_PIXEL_FAST(row_ptr, x) IMAGE_GET_RGB565_PIXEL_FAST((row_ptr), ((size_t) ((IMAGE_X_RATIO * ((x) - IMAGE_X_TARGET_OFFSET)) + 0.5)) + IMAGE_X_SOURCE_OFFSET)
// #define IMAGE_GET_SCALED_RGB888_PIXEL_FAST(row_ptr, x) IMAGE_GET_RGB888_PIXEL_FAST((row_ptr), ((size_t) ((IMAGE_X_RATIO * ((x) - IMAGE_X_TARGET_OFFSET)) + 0.5)) + IMAGE_X_SOURCE_OFFSET)

// // Old Image Macros - Will be refactor and removed. But, only after making sure through testing new macros work.

// #define IM_SWAP16(x) __REV16(x) // Swap bottom two chars in short.
// #define IM_SWAP32(x) __REV32(x) // Swap bottom two shorts in long.

// RGB565 to RGB888 conversion.
extern const uint8_t rb528_table[32];
extern const uint8_t g628_table[64];

#define IM_R528(p) \
    ({ __typeof__ (p) _p = (p); \
       rb528_table[_p]; })

#define IM_G628(p) \
    ({ __typeof__ (p) _p = (p); \
       g628_table[_p]; })

#define IM_B528(p) \
    ({ __typeof__ (p) _p = (p); \
       rb528_table[_p]; })

// RGB888 to RGB565 conversion.
extern const uint8_t rb825_table[256];
extern const uint8_t g826_table[256];

#define IM_R825(p) \
    ({ __typeof__ (p) _p = (p); \
       rb825_table[_p]; })

#define IM_G826(p) \
    ({ __typeof__ (p) _p = (p); \
       g826_table[_p]; })

#define IM_B825(p) \
    ({ __typeof__ (p) _p = (p); \
       rb825_table[_p]; })

// Split RGB565 values (note the RGB565 value is byte reversed).

// #define IM_R565(p) \
//     ({ __typeof__ (p) _p = (p); \
//        ((_p)>>3)&0x1F; })

// #define IM_G565(p) \
//     ({ __typeof__ (p) _p = (p); \
//        (((_p)&0x07)<<3)|((_p)>>13); })

// #define IM_B565(p) \
//     ({ __typeof__ (p) _p = (p); \
//        ((_p)>>8)&0x1F; })

// // Merge RGB565 values (note the RGB565 value is byte reversed).

// #define IM_RGB565(r, g, b) \
//     ({ __typeof__ (r) _r = (r); \
//        __typeof__ (g) _g = (g); \
//        __typeof__ (b) _b = (b); \
//        ((_r)<<3)|((_g)>>3)|((_g)<<13)|((_b)<<8); })

// // Image kernels
// extern const int8_t kernel_gauss_3[9];
// extern const int8_t kernel_gauss_5[25];
// extern const int kernel_laplacian_3[9];
// extern const int kernel_high_pass_3[9];

// #define IM_RGB5652L(p) \
//     ({ __typeof__ (p) _p = (p); \
//        lab_table[_p * 3]; })

// #define IM_RGB5652A(p) \
//     ({ __typeof__ (p) _p = (p); \
//        lab_table[(_p * 3) + 1]; })

// #define IM_RGB5652B(p) \
//     ({ __typeof__ (p) _p = (p); \
//        lab_table[(_p * 3) + 2]; })

// #define IM_RGB8882L(p) \
//     ({ __typeof__ (p) _p = (p); \
//        lab_table[_p * 3]; })

// #define IM_RGB8882A(p) \
//     ({ __typeof__ (p) _p = (p); \
//        lab_table[(_p * 3) + 1]; })

// #define IM_RGB8882B(p) \
//     ({ __typeof__ (p) _p = (p); \
//        lab_table[(_p * 3) + 2]; })
// // Grayscale maxes
// #define IM_MAX_GS (255)

// // RGB565 maxes
// #define IM_MAX_R5 (31)
// #define IM_MAX_G6 (63)
// #define IM_MAX_B5 (31)

// // RGB888 maxes
// #define IM_MAX_R5 (255)
// #define IM_MAX_G6 (255)
// #define IM_MAX_B5 (255)

// // Grayscale histogram
// #define IM_G_HIST_SIZE (256)
// #define IM_G_HIST_OFFSET (0)

// // LAB histogram
// #define IM_L_HIST_SIZE (256)
// #define IM_L_HIST_OFFSET (0)
// #define IM_A_HIST_SIZE (256)
// #define IM_A_HIST_OFFSET (256)
// #define IM_B_HIST_SIZE (256)
// #define IM_B_HIST_OFFSET (512)

// #define IM_IS_BINARY(img) \
//     ({ __typeof__ (img) _img = (img); \
//        _img->bpp == 0; })

// #define IM_IS_GS(img) \
//     ({ __typeof__ (img) _img = (img); \
//        _img->bpp == 1; })

// #define IM_IS_RGB565(img) \
//     ({ __typeof__ (img) _img = (img); \
//        _img->bpp == 2; })

// #define IM_IS_RGB888(img) \
//     ({ __typeof__ (img) _img = (img); \
//        _img->bpp == 5; })

// #define IM_IS_RGBA8888(img) \
//     ({ __typeof__ (img) _img = (img); \
//        _img->bpp == 6; })

// #define IM_IS_BAYER(img) \
//     ({ __typeof__ (img) _img = (img); \
//        _img->bpp == 3; })

// #define IM_IS_JPEG(img) \
//     ({ __typeof__ (img) _img = (img); \
//        _img->bpp >= 4; })

// #define IM_IS_MUTABLE(img) \
//     ({ __typeof__ (img) _img = (img); \
//        (_img->bpp == 1 || _img->bpp == 2); })

// #define IM_X_INSIDE(img, x) \
//     ({ __typeof__ (img) _img = (img); \
//        __typeof__ (x) _x = (x); \
//        (0<=_x)&&(_x<_img->w); })

// #define IM_Y_INSIDE(img, y) \
//     ({ __typeof__ (img) _img = (img); \
//        __typeof__ (y) _y = (y); \
//        (0<=_y)&&(_y<_img->h); })

// #define IM_GET_GS_PIXEL(img, x, y) \
//     ({ __typeof__ (img) _img = (img); \
//        __typeof__ (x) _x = (x); \
//        __typeof__ (y) _y = (y); \
//        ((uint8_t*)_img->pixels)[(_y*_img->w)+_x]; })

// #define IM_GET_RAW_PIXEL(img, x, y) \
//     ({ __typeof__ (img) _img = (img); \
//        __typeof__ (x) _x = (x); \
//        __typeof__ (y) _y = (y); \
//        ((uint8_t*)_img->pixels)[(_y*_img->w)+_x]; })

// #define IM_GET_RAW_PIXEL_CHECK_BOUNDS_X(img, x, y) \
//     ({ __typeof__ (img) _img = (img); \
//        __typeof__ (x) _x = (x); \
//        __typeof__ (y) _y = (y); \
//        _x = (_x < 0) ? 0 : (_x >= img->w) ? (img->w -1): _x; \
//        ((uint8_t*)_img->pixels)[(_y*_img->w)+_x]; })

// #define IM_GET_RAW_PIXEL_CHECK_BOUNDS_Y(img, x, y) \
//     ({ __typeof__ (img) _img = (img); \
//        __typeof__ (x) _x = (x); \
//        __typeof__ (y) _y = (y); \
//        _y = (_y < 0) ? 0 : (_y >= img->h) ? (img->h -1): _y; \
//        ((uint8_t*)_img->pixels)[(_y*_img->w)+_x]; })

// #define IM_GET_RAW_PIXEL_CHECK_BOUNDS_XY(img, x, y) \
//     ({ __typeof__ (img) _img = (img); \
//        __typeof__ (x) _x = (x); \
//        __typeof__ (y) _y = (y); \
//        _x = (_x < 0) ? 0 : (_x >= img->w) ? (img->w -1): _x; \
//        _y = (_y < 0) ? 0 : (_y >= img->h) ? (img->h -1): _y; \
//        ((uint8_t*)_img->pixels)[(_y*_img->w)+_x]; })

// #define IM_GET_RGB565_PIXEL(img, x, y) \
//     ({ __typeof__ (img) _img = (img); \
//        __typeof__ (x) _x = (x); \
//        __typeof__ (y) _y = (y); \
//        ((uint16_t*)_img->pixels)[(_y*_img->w)+_x]; })

// #define IM_GET_RGB888_PIXEL(img, x, y) \
//     ({ __typeof__ (img) _img = (img); \
//        __typeof__ (x) _x = (x); \
//        __typeof__ (y) _y = (y); \
//        ((uint16_t*)_img->pixels)[(_y*_img->w)+_x]; })

// #define IM_SET_GS_PIXEL(img, x, y, p) \
//     ({ __typeof__ (img) _img = (img); \
//        __typeof__ (x) _x = (x); \
//        __typeof__ (y) _y = (y); \
//        __typeof__ (p) _p = (p); \
//        ((uint8_t*)_img->pixels)[(_y*_img->w)+_x]=_p; })

// #define IM_SET_RGB565_PIXEL(img, x, y, p) \
//     ({ __typeof__ (img) _img = (img); \
//        __typeof__ (x) _x = (x); \
//        __typeof__ (y) _y = (y); \
//        __typeof__ (p) _p = (p); \
//        ((uint16_t*)_img->pixels)[(_y*_img->w)+_x]=_p; })

// #define IM_SET_RGB888_PIXEL(img, x, y, p) \
//     ({ __typeof__ (img) _img = (img); \
//        __typeof__ (x) _x = (x); \
//        __typeof__ (y) _y = (y); \
//        __typeof__ (p) _p = (p); \
//        ((uint16_t*)_img->pixels)[(_y*_img->w)+_x]=_p; })

// #define IM_EQUAL(img0, img1) \
//     ({ __typeof__ (img0) _img0 = (img0); \
//        __typeof__ (img1) _img1 = (img1); \
//        (_img0->w==_img1->w)&&(_img0->h==_img1->h)&&(_img0->bpp==_img1->bpp); })

// #ifdef IMLIB_ENABLE_YUV_LAB_FUNC

// #define IM_TO_GS_PIXEL(img, x, y)    \
//     (img->bpp == 1 ? img->pixels[((y)*img->w)+(x)] : (yuv_table(((uint16_t*)img->pixels)[((y)*img->w)+(x)] * 3) + 128))

// #else

// #define IM_TO_GS_PIXEL(img, x, y)    \
//     (img->bpp == 1 ? img->pixels[((y)*img->w)+(x)] : (yuv_table[((uint16_t*)img->pixels)[((y)*img->w)+(x)] * 3] + 128))

// #endif




int checkCPU(void);

#endif