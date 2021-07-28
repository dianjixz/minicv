/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * Memory allocation functions.
 *
 */
//#include <mp.h>
#include<stdlib.h>
#include<stdio.h>
#include "xalloc.h"

#ifndef NORETURN
#define NORETURN __attribute__((noreturn))
#endif

NORETURN static void xalloc_fail()
{
    sprintf(stderr, "Out of normal MicroPython Heap Memory! Please reduce the resolution of the image you are running this algorithm on to bypass this issue!\r\n");
}

// returns null pointer without error if size==0
void *xalloc(uint32_t size)
{
    void *mem = malloc(size);
    if (size && (mem == NULL)) {
        xalloc_fail();
    }
    return mem;
}

// returns null pointer without error if size==0
void *xalloc_try_alloc(uint32_t size)
{
    return malloc(size);
}

// returns null pointer without error if size==0
void *xalloc0(uint32_t size)
{
    void *mem = malloc(size);
    if (size && (mem == NULL)) {
        xalloc_fail();
    }
    memset(mem, 0, size);
    return mem;
}

// returns without error if mem==null
void xfree(void *mem)
{
    free(mem);
}

// returns null pointer without error if size==0
// allocs if mem==null and size!=0
// frees if mem!=null and size==0
void *xrealloc(void *mem, uint32_t size)
{
    mem = realloc(mem, size);
    if (size && (mem == NULL)) {
        xalloc_fail();
    }
    return mem;
}
