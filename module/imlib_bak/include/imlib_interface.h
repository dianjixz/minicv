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


int checkCPU(void);

#endif