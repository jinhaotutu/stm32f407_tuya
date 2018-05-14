/**
  ******************************************************************************
  * @file    main.c 
  * @author  jinhao
  * @version V1.0.0
  * @date    22-April-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Includes -------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "sys_cfg.h"
#include "sys_delay.h"

#include "wifi.h"
#include "net_device.h"
#include "net_io.h"
#include "SI7021.h"
#include "oled.h"
#include "include.h"

/* Defines --------------------------------------------------------------------*/


/* Variables ------------------------------------------------------------------*/
TYPE_BUFFER_S FlashBuffer;
				
/* Functions ------------------------------------------------------------------*/			



/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    STMFLASH_Read(PARA_ADDR, (unsigned int *)&FlashBuffer, (sizeof(FlashBuffer)/4+1));

    if((FlashBuffer.magic_code != 0) || (FlashBuffer.magic_code != 0xffffffff))
    {
        memset(&FlashBuffer,0,sizeof(FlashBuffer));
        FlashBuffer.bright_value = 0xff;
    
        STMFLASH_Write(PARA_ADDR,(unsigned int *)&FlashBuffer,(sizeof(FlashBuffer)/4+1));
    }
    
	/* stm32系统配置 */
	Sys_Config();
    
  OLED_Init();
	
	SI_IO_Init();
	
	NET_DEVICE_IO_Init();
	
	wifi_protocol_init();

//	mcu_set_wifi_mode(SMART_CONFIG);
//	mcu_set_wifi_mode(AP_CONFIG);
		
	while(1)
	{
		wifi_uart_service();
		wifi_state = mcu_get_wifi_work_state();
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
