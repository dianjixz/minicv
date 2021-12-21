/*
 * This file is part of the OpenMV project.
 *
 * Copyright (c) 2013-2021 Ibrahim Abdelkader <iabdalkader@openmv.io>
 * Copyright (c) 2013-2021 Kwabena W. Agyeman <kwagyeman@openmv.io>
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * Memory allocation functions.
 */
#include <string.h>
// #include "py/runtime.h"
// #include "py/gc.h"
// #include "py/mphal.h"
#include "xalloc.h"

static void xalloc_fail(uint32_t size)
{
    printf("nihao\n");
    // mp_raise_msg_varg(&mp_type_MemoryError,
    //         MP_ERROR_TEXT("memory allocation failed, allocating %u bytes"), (uint)size);
}

// returns null pointer without error if size==0
void *xalloc(uint32_t size)
{
    void *mem = malloc(size);
    if (size && (mem == NULL)) {
        xalloc_fail(size);
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
        xalloc_fail(size);
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
        xalloc_fail(size);
    }
    return mem;
}
