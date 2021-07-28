#ifndef __IMDEFS_H
#define __IMDEFS_H
typedef unsigned char uint8_t;
typedef unsigned char uint8_t;


typedef struct image {
    int w;
    int h;
    int bpp;
    union{
        uint8_t *pixels;
        uint8_t *data;
    };
	uint8_t *pix_ai;	//for MAIX AI speed up
} __attribute__((aligned(8)))image_t;

typedef struct image_py {
    int w;
    int h;
    int bpp;
    uint8_t *data;
	uint8_t *pix_ai;	//for MAIX AI speed up
} __attribute__((aligned(8)))image_t_py;








#endif
