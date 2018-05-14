/****************************************Copyright (c)*************************
**                               版权所有 (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**文   件   名: protocol.c
**描        述: 下发/上报数据处理函数
**使 用 说 明 :

                  *******非常重要，一定要看哦！！！********

** 1、用户在此文件中实现数据下发/上报功能
** 2、DP的ID/TYPE及数据处理函数都需要用户按照实际定义实现
** 3、当开始某些宏定义后需要用户实现代码的函数内部有#err提示,完成函数后请删除该#err
**
**--------------当前版本修订---------------------------------------------------
** 版  本: v2.3.8
** 日　期: 2018年1月17日
** 描　述: 1:变量添加volatile防止编译器优化
           2:添加#error提示
		   
** 版  本: v2.3.7
** 日　期: 2017年4月18日
** 描　述: 1:优化串口队列接收处理
		   
** 版  本: v2.3.6
** 日　期: 2016年7月21日
** 描　述: 1:修复获取本地时间错误
           2:添加hex_to_bcd转换函数
		   
** 版  本: v2.3.5
** 日　期: 2016年6月3日
** 描　述: 1:修改返回协议版本为0x01
           2:固件升级数据偏移量修改为4字节

** 版  本: v2.3.4
** 日　期: 2016年5月26日
** 描　述: 1:优化串口解析函数
           2:优化编译器兼容性,取消enum类型定义

** 版  本: v2.3.3
** 日　期: 2016年5月24日
** 描　述: 1:修改mcu获取本地时间函数
           2:添加wifi功能测试

** 版  本: v2.3.2
** 日　期: 2016年4月23日
** 描　述: 1:优化串口数据解析
           2:优化MCU固件升级流程
           3:优化上报流程

** 版  本: v2.3.1
** 日　期: 2016年4月15日
** 描　述: 1:优化串口数据解析

** 版  本: v2.3
** 日　期: 2016年4月14日
** 描　述: 1:支持MCU固件在线升级

** 版  本: v2.2
** 日　期: 2016年4月11日
** 描　述: 1:修改串口数据接收方式

** 版  本: v2.1
** 日　期: 2016年4月8日
** 描　述: 1:加入某些编译器不支持函数指针兼容选项

** 版  本: v2.0
** 日　期: 2016年3月29日
** 描　述: 1:优化代码结构
           2:节省RAM空间
**
**-----------------------------------------------------------------------------
******************************************************************************/
#include "sys_cfg.h"
#include "SI7021.h"
#include "oled.h"

#include "wifi.h"
#include "include.h"

extern TYPE_BUFFER_S FlashBuffer;

/******************************************************************************
                                移植须知:
1:MCU必须在while中直接调用mcu_api.c内的wifi_uart_service()函数
2:程序正常初始化完成后,建议不进行关串口中断,如必须关中断,关中断时间必须短,关中断会引起串口数据包丢失
3:请勿在中断/定时器中断内调用上报函数
******************************************************************************/

         
/******************************************************************************
                              第一步:初始化
1:在需要使用到wifi相关文件的文件中include "wifi.h"
2:在MCU初始化中调用mcu_api.c文件中的wifi_protocol_init()函数
3:将MCU串口单字节发送函数填入protocol.c文件中uart_transmit_output函数内,并删除#error
4:在MCU串口接收函数中调用mcu_api.c文件内的uart_receive_input函数,并将接收到的字节作为参数传入
5:单片机进入while循环后调用mcu_api.c文件内的wifi_uart_service()函数
******************************************************************************/

/******************************************************************************
                        1:dp数据点序列类型对照表
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/
DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_POWER, DP_TYPE_BOOL, 0},
  {DPID_TEMPSET, DP_TYPE_VALUE, 25},
  {DPID_TEMPCURRENT, DP_TYPE_VALUE, 22},
  {DPID_MODE, DP_TYPE_ENUM, 0},
  {DPID_LIGHT, DP_TYPE_BOOL, 0},
  {DPID_FAULT, DP_TYPE_FAULT, 0},
};


/******************************************************************************
                           2:串口单字节发送函数
请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
******************************************************************************/

/*****************************************************************************
函数名称 : uart_transmit_output
功能描述 : 发数据处理
输入参数 : value:串口收到字节数据
返回参数 : 无
使用说明 : 请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
*****************************************************************************/
#include "net_io.h"
void uart_transmit_output(unsigned char value)
{
//  #error "请将MCU串口发送函数填入该函数,并删除该行"
	
	NET_IO_Send(&value,1);
	
/*
  //示例:
  extern void Uart_PutChar(unsigned char value);
  Uart_PutChar(value);	                                //串口发送函数
*/
}
/******************************************************************************
                           第二步:实现具体用户函数
1:APP下发数据处理
2:数据上报处理
******************************************************************************/

/******************************************************************************
                            1:所有数据上报处理
当前函数处理全部数据上报(包括可下发/可上报和只上报)
  需要用户按照实际情况实现:
  1:需要实现可下发/可上报数据点上报
  2:需要实现只上报数据点上报
此函数为MCU内部必须调用
用户也可调用此函数实现全部数据上报
******************************************************************************/

//自动化生成数据上报函数

/*****************************************************************************
函数名称 : all_data_update
功能描述 : 系统所有dp点信息上传,实现APP和muc数据同步
输入参数 : 无
返回参数 : 无
使用说明 : 此函数SDK内部需调用;
           MCU必须实现该函数内数据上报功能;包括只上报和可上报可下发型数据
*****************************************************************************/
void all_data_update(void)
{
//  #error "请在此处理可下发可上报数据及只上报数据示例,处理完成后删除该行"
	SI7021_measure();
	
  //此代码为平台自动生成，请按照实际数据修改每个可下发可上报函数和只上报函数
  mcu_dp_bool_update(DPID_POWER, download_cmd[0].dp_value); //BOOL型数据上报;
  mcu_dp_value_update(DPID_TEMPSET, download_cmd[1].dp_value); //VALUE型数据上报;
  mcu_dp_value_update(DPID_TEMPCURRENT,download_cmd[2].dp_value); //VALUE型数据上报;
  mcu_dp_enum_update(DPID_MODE,download_cmd[3].dp_value); //枚举型数据上报;
  mcu_dp_bool_update(DPID_LIGHT,download_cmd[4].dp_value); //BOOL型数据上报;
  mcu_dp_fault_update(DPID_FAULT,download_cmd[5].dp_value); //故障型数据上报;
    
  oled_tuya_dis();

}


/******************************************************************************
                                WARNING!!!    
                            2:所有数据上报处理
自动化代码模板函数,具体请用户自行实现数据处理
******************************************************************************/


/*****************************************************************************
函数名称 : dp_download_power_handle
功能描述 : 针对DPID_POWER的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_power_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为BOOL
  unsigned char ret;
  //0:关/1:开
  unsigned char power;
  
  power = mcu_get_dp_download_bool(value,length);
    download_cmd[0].dp_value = power;
    oled_tuya_dis();
  if(power == 0)
  {
    //开关关
		SI7021_POWER_OFF;
  }
  else
  {
    //开关开
		SI7021_POWER_ON;
  }
  
  //处理完DP数据后应有反馈
  ret = mcu_dp_bool_update(DPID_POWER,power);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_tempset_handle
功能描述 : 针对DPID_TEMPSET的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_tempset_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为VALUE
  unsigned char ret;
  unsigned long tempset;
  
  tempset = mcu_get_dp_download_value(value,length);

  //VALUE类型数据处理
  download_cmd[1].dp_value = tempset;
  oled_tuya_dis();

  
  //处理完DP数据后应有反馈
  ret = mcu_dp_value_update(DPID_TEMPSET,tempset);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_mode_handle
功能描述 : 针对DPID_MODE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_mode_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为ENUM
  unsigned char ret;
  unsigned char mode;
  
  mode = mcu_get_dp_download_enum(value,length);
	download_cmd[3].dp_value = mode;
    oled_tuya_dis();
  switch(mode)
  {
    case 0:
      
      break;
      
    case 1:
      
      break;
      
    case 2:
      
      break;
      
    default:
      
      break;
  }
  
  //处理完DP数据后应有反馈
  ret = mcu_dp_enum_update(DPID_MODE,mode);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_light_handle
功能描述 : 针对DPID_LIGHT的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_light_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为BOOL
  unsigned char ret;
  //0:关/1:开
  unsigned char light;
  
  light = mcu_get_dp_download_bool(value,length);
	download_cmd[4].dp_value = light;
    oled_tuya_dis();
  if(light == 0)
  {
    //开关关
		LED2_OFF;
  }
  else
  {
    //开关开
		LED2_ON;
  }
  
  //处理完DP数据后应有反馈
  ret = mcu_dp_bool_update(DPID_LIGHT,light);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}


/******************************************************************************
                                WARNING!!!                     
此代码为SDK内部调用,请按照实际dp数据实现函数内部数据
******************************************************************************/
#ifdef SUPPORT_MCU_RTC_CHECK
/*****************************************************************************
函数名称 : mcu_write_rtctime
功能描述 : MCU校对本地RTC时钟
输入参数 : 无
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void mcu_write_rtctime(unsigned char time[])
{
  #error "请自行完成RTC时钟写入代码,并删除该行"
  /*
  time[0]为是否获取时间成功标志，为 0 表示失败，为 1表示成功
  time[1] 为 年 份 , 0x00 表 示2000 年
  time[2]为月份，从 1 开始到12 结束
  time[3]为日期，从 1 开始到31 结束
  time[4]为时钟，从 0 开始到23 结束
  time[5]为分钟，从 0 开始到59 结束
  time[6]为秒钟，从 0 开始到59 结束
  time[7]为星期，从 1 开始到 7 结束，1代表星期一
 */
  if(time[0] == 1)
  {
    //正确接收到wifi模块返回的本地时钟数据 
	 
  }
  else
  {
  	//获取本地时钟数据出错,有可能是当前wifi模块未联网
  }
}
#endif

#ifdef WIFI_TEST_ENABLE
/*****************************************************************************
函数名称 : wifi_test_result
功能描述 : wifi功能测试反馈
输入参数 : result:wifi功能测试结果;0:失败/1:成功
           rssi:测试成功表示wifi信号强度/测试失败表示错误类型
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void wifi_test_result(unsigned char result,unsigned char rssi)
{
//  #error "请自行实现wifi功能测试成功/失败代码,完成后请删除该行"
  if(result == 0)
  {
    //测试失败
    if(rssi == 0x00)
    {
      //未扫描到名称为tuya_mdev_test路由器,请检查
    }
    else if(rssi == 0x01)
    {
      //模块未授权
    }
  }
  else
  {
    //测试成功
    //rssi为信号强度(0-100, 0信号最差，100信号最强)
  }
  
}
#endif

#ifdef SUPPORT_MCU_FIRM_UPDATE
/*****************************************************************************
函数名称 : mcu_firm_update_handle
功能描述 : MCU进入固件升级模式
输入参数 : value:固件缓冲区
           position:当前数据包在于固件位置
           length:当前固件包长度(固件包长度为0时,表示固件包发送完成)
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
//  #error "请自行完成MCU固件升级代码,完成后请删除该行"
    unsigned long adder;
    
  if(length == 0)
  {
#ifdef ENABLE_BOOT
    //固件数据发送完成
    FlashBuffer.magic_code = FIREWARE_UPDATE_FLAG;
    
    STMFLASH_Write(PARA_ADDR,(unsigned int *)&FlashBuffer,(sizeof(FlashBuffer)/4+1));
    
    NVIC_SystemReset();
#endif
    
  }
  else
  {
    //固件数据处理
      adder = FIREWARE_ADDR_H;
      
      if (length%4)
      {
          length = length/4 + 1;
      }
      else
      {
          length = length/4;
      }
      
      STMFLASH_Write(adder + position, (unsigned int *)value, length);
  }
  
  return SUCCESS;
}
#endif
/******************************************************************************
                                WARNING!!!                     
以下函数用户请勿修改!!
******************************************************************************/

/*****************************************************************************
函数名称 : dp_download_handle
功能描述 : dp下发处理函数
输入参数 : dpid:DP序号
           value:dp数据缓冲区地址
           length:dp数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERRO
使用说明 : 该函数用户不能修改
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /*********************************
  当前函数处理可下发/可上报数据调用                    
  具体函数内需要实现下发数据处理
  完成用需要将处理结果反馈至APP端,否则APP会认为下发失败
  ***********************************/
  unsigned char ret;
  switch(dpid)
  {
    case DPID_POWER:
      //开关处理函数
      ret = dp_download_power_handle(value,length);
      break;
    case DPID_TEMPSET:
      //目标温度处理函数
      ret = dp_download_tempset_handle(value,length);
      break;
    case DPID_MODE:
      //模式处理函数
      ret = dp_download_mode_handle(value,length);
      break;
    case DPID_LIGHT:
      //灯光处理函数
      ret = dp_download_light_handle(value,length);
      break;

  default:
    break;
  }
  return ret;
}
/*****************************************************************************
函数名称 : get_download_cmd_total
功能描述 : 获取所有dp命令总和
输入参数 : 无
返回参数 : 下发命令总和
使用说明 : 该函数用户不能修改
*****************************************************************************/
unsigned char get_download_cmd_total(void)
{
  return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}
