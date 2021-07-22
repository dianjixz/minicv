#ifndef __MINICVCONFIG_H__
#define __MINICVCONFIG_H__

#define DBGLOG_TRACE(format, ...)

#define DBGLOG_DEBUG(format, ...)


#define DBGLOG_ERROR(format, ...)
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






















#endif
