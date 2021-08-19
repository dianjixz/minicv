/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * Board configuration and pin definitions.
 *
 */
#ifndef __OMV_BOARDCONFIG_H__
#define __OMV_BOARDCONFIG_H__

#include "imlib_config.h"
// #include "lcd.h"
// #include "global_config.h"

#define OMV_INIT_BPP 2

// IDE config
#define IDE_DBG_MAX_PACKET  64
#define IDE_DBG_CMD_START_FLAG 0x30

#if !defined(OMV_MINIMUM) || CONFIG_MAIXPY_IDE_SUPPORT
  #define OMV_JPEG_BUF_SIZE 512 * 1024 // IDE JPEG buffer (header + data).
#endif

#ifndef OMV_MINIMUM
#define OMV_FB_ALLOC_SIZE 1024 * 1024 // minimum fb alloc size
#define FB_MAX_ALLOC_TIMES    250

#else  //OMV_MINIMUM

#define OMV_FB_ALLOC_SIZE 300 * 1024 // minimum fb alloc size
#define FB_MAX_ALLOC_TIMES    50

#endif //OMV_MINIMUM

// #include "dvp.h"
#define DCMI_RESET_LOW()      dvp->cmos_cfg &= ~DVP_CMOS_RESET
#define DCMI_RESET_HIGH()     dvp->cmos_cfg |= DVP_CMOS_RESET
#define DCMI_PWDN_LOW()       dvp->cmos_cfg &= ~DVP_CMOS_POWER_DOWN
#define DCMI_PWDN_HIGH()      dvp->cmos_cfg |= DVP_CMOS_POWER_DOWN


// Architecture info
#define OMV_ARCH_STR            "K210" // 33 chars max
#define OMV_BOARD_TYPE          "Maix"

// Flash sectors for the bootloader.
// Flash FS sector, main FW sector, max sector.
#define OMV_FLASH_LAYOUT        {1, 2, 15}

#define OMV_XCLK_MCO            (0U)
#define OMV_XCLK_TIM            (1U)

// Sensor external clock source.
#define OMV_XCLK_SOURCE         (OMV_XCLK_TIM)

// Sensor external clock timer frequency.
// #define OMV_XCLK_FREQUENCY      (12000000)
#define OMV_XCLK_FREQUENCY      (CONFIG_SENSOR_FREQ)
// Sensor PLL register value.
#define OMV_OV7725_PLL_CONFIG   (0x41)  // x4
#define OMV_OV7725_BANDING      (0x3F)  //TODO:

// Bootloader LED GPIO port/pin
#define OMV_BOOTLDR_LED_PIN     (GPIO_PIN_1)
#define OMV_BOOTLDR_LED_PORT    (GPIOC)

// RAW buffer size
#define OMV_RAW_BUF_SIZE        (MAIN_FB()->w_max * MAIN_FB()->h_max * OMV_INIT_BPP)

// Enable hardware JPEG
#define OMV_HARDWARE_JPEG       (0)

// Enable MT9V034 and LEPTON sensors
#define OMV_ENABLE_MT9V034      (1)
#define OMV_ENABLE_LEPTON       (1)

// If buffer size is bigger than this threshold, the quality is reduced.
// This is only used for JPEG images sent to the IDE not normal compression.
#define JPEG_QUALITY_THRESH     (320*240*2)

// Low and high JPEG QS.
#define JPEG_QUALITY_LOW        50
#define JPEG_QUALITY_HIGH       90

// Linker script constants (see the linker script template stm32fxxx.ld.S).
// Note: fb_alloc is a stack-based, dynamically allocated memory on FB.
// The maximum available fb_alloc memory = FB_ALLOC_SIZE + FB_SIZE - (w*h*bpp).
#define OMV_FFS_MEMORY          CCM         // Flash filesystem cache memory
#define OMV_MAIN_MEMORY         SRAM1       // data, bss, stack and heap
#define OMV_DMA_MEMORY          AXI_SRAM    // DMA buffers memory.
#define OMV_FB_MEMORY           AXI_SRAM    // Framebuffer, fb_alloc
#define OMV_JPEG_MEMORY         SRAM3       // JPEG buffer memory.
#define OMV_VOSPI_MEMORY        SRAM4       // VoSPI buffer memory.

#define OMV_STACK_SIZE          (7K)
#define OMV_HEAP_SIZE           (240K)

#define OMV_MSC_BUF_SIZE        (12K)       // USB MSC bot data
#define OMV_VFS_BUF_SIZE        (1K)        // VFS sturct + FATFS file buffer (624 bytes)

#define OMV_BOOT_ORIGIN         0x08000000
#define OMV_BOOT_LENGTH         128K
#define OMV_TEXT_ORIGIN         0x08040000
#define OMV_TEXT_LENGTH         1792K
#define OMV_CCM_ORIGIN          0x20000000  // Note accessible by CPU and MDMA only.
#define OMV_CCM_LENGTH          128K
#define OMV_SRAM1_ORIGIN        0x30000000
#define OMV_SRAM1_LENGTH        256K
#define OMV_SRAM3_ORIGIN        0x30040000
#define OMV_SRAM3_LENGTH        32K
#define OMV_SRAM4_ORIGIN        0x38000000
#define OMV_SRAM4_LENGTH        64K
#define OMV_AXI_SRAM_ORIGIN     0x24000000
#define OMV_AXI_SRAM_LENGTH     512K

// Use the MPU to set an uncacheable memory region.
#define OMV_DMA_REGION_BASE     (OMV_AXI_SRAM_ORIGIN+(496*1024))
#define OMV_DMA_REGION_SIZE     MPU_REGION_SIZE_16KB




// For K210
#define __CLZ(n) __builtin_clz(n)
/**
  \brief   Reverse byte order (16 bit)
  \details Reverses the byte order in two unsigned short values.
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
#define __REV16          __builtin_bswap16                           /* ToDo:  ARMCC_V6: check if __builtin_bswap16 could be used */

#endif //__OMV_BOARDCONFIG_H__
