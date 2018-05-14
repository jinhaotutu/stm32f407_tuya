#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include	<stdio.h>
#include	<math.h>
#include	<stdlib.h>
#include	<string.h>
//#include 	<intrins.h>
#include	<ctype.h>
#include    "stm32f4xx.h"

#define         ENABLE_BOOT                             //使能BOOT(固件升级功能)
//(7+1K)+28K+28K
#define         BASIC_FLASH_ADDR                        0x08000000

#ifdef ENABLE_BOOT
  #define         OFFSET_PARA                             0x20000        
#else
  #define         OFFSET_PARA                             0x20000         
#endif

#define         OFFSET_FIRMWARE_L                       0x4000                  //固件正常运行地址
#define         OFFSET_FIRMWARE_H                       0x10000                  //升级固件存储地址

#define         PARA_ADDR                               (BASIC_FLASH_ADDR + OFFSET_PARA)
#define         FIREWARE_ADDR_L                         (BASIC_FLASH_ADDR + OFFSET_FIRMWARE_L)   //
#define         FIREWARE_ADDR_H                         (BASIC_FLASH_ADDR + OFFSET_FIRMWARE_H)   //

#define         FIREWARE_UPDATE_FLAG                           0x55555555

#include "stmflash.h"

typedef struct {
  uint32_t magic_code;
  uint8_t led_switch;
  uint8_t work_mode;
  uint8_t bright_value;
  uint8_t colour[14];
  uint8_t scene[14];
} TYPE_BUFFER_S;

#endif
