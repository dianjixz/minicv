#include "minicvconfig.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define random(x) (rand()%x)

#include <time.h>
#include <sys/time.h>


char *minicv_fb_base;
char *minicv_jpeg_buf;
char *_fballoc;
char *_fb_base;
char *_jpeg_buf;
void imlib_init()
{
	minicv_fb_base = (char *)malloc(OMV_FB_SIZE + OMV_FB_ALLOC_SIZE + 1);
	_fballoc = minicv_fb_base + OMV_FB_SIZE + OMV_FB_ALLOC_SIZE;
	_fb_base = minicv_fb_base;
	minicv_jpeg_buf = (char *)malloc(OMV_JPEG_BUF_SIZE);
	_jpeg_buf = minicv_jpeg_buf;
	fb_alloc_init0();
    framebuffer_init0();
}
void __DMB()
{

}
void __disable_irq()
{

}
void __enable_irq()
{

}
void __LDREXW()
{

}
void __STREXW()
{

}
void __WFI()
{

}
long getTimems()
{
    long tic;
    struct timeval t;
    gettimeofday(&t, 0);
     
    tic = (long)((long)t.tv_sec * 1000 * 1000 + t.tv_usec);
    return tic/1000;
}
long mp_hal_ticks_ms()
{
	return getTimems();
}

uint32_t rng_randint(uint32_t min, uint32_t max)
{
	srand((int)time(0));
	uint32_t a;
	do
	{
		a = random(max);
	} while (a > min && a < max);
	return a;
}
