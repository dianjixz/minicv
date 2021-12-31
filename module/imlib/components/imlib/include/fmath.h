/*
 * This file is part of the OpenMV project.
 *
 * Copyright (c) 2013-2021 Ibrahim Abdelkader <iabdalkader@openmv.io>
 * Copyright (c) 2013-2021 Kwabena W. Agyeman <kwagyeman@openmv.io>
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * Fast approximate math functions.
 */
#ifndef __FMATH_H__
#define __FMATH_H__
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#ifdef __cplusplus
extern "C"
{
#endif

#if (__ARM_ARCH < 7)
#include <math.h>

#define fast_sqrtf sqrtf
#define fast_floorf(_x) (int)floorf(_x)
#define fast_ceilf ceilf
#define fast_roundf(_x) (int)roundf(_x)
#define fast_fabsf fabsf

#else
float fast_sqrtf(float x);
int fast_floorf(float x);
int fast_ceilf(float x);
int fast_roundf(float x);
float fast_fabsf(float x);
#endif

float fast_atanf(float x);
float fast_atan2f(float y, float x);
float fast_expf(float x);
float fast_cbrtf(float d);
float fast_log(float x);
float fast_log2(float x);
float fast_powf(float a, float b);
void fast_get_min_max(float *data, size_t data_len, float *p_min, float *p_max);
extern const float cos_table[360];
extern const float sin_table[360];
#ifdef __cplusplus
}
#endif
#endif // __FMATH_H__
