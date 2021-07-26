#include "minicvconfig.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define random(x) (rand()%x)

#include <time.h>
#include <sys/time.h>
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
