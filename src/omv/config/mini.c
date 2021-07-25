#include "minicvconfig.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define random(x) (rand()%x)


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
void mp_hal_ticks_ms()
{
	
}



// void main()
// {
	
// 	for (int x = 0; x < 10; x++)
// 		printf("%d\n", random(100));
// }

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
