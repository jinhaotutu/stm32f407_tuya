/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: protocol.c
**��        ��: �·�/�ϱ����ݴ�����
**ʹ �� ˵ �� :

                  *******�ǳ���Ҫ��һ��Ҫ��Ŷ������********

** 1���û��ڴ��ļ���ʵ�������·�/�ϱ�����
** 2��DP��ID/TYPE�����ݴ���������Ҫ�û�����ʵ�ʶ���ʵ��
** 3������ʼĳЩ�궨�����Ҫ�û�ʵ�ִ���ĺ����ڲ���#err��ʾ,��ɺ�������ɾ����#err
**
**--------------��ǰ�汾�޶�---------------------------------------------------
** ��  ��: v2.3.8
** �ա���: 2018��1��17��
** �衡��: 1:�������volatile��ֹ�������Ż�
           2:���#error��ʾ
		   
** ��  ��: v2.3.7
** �ա���: 2017��4��18��
** �衡��: 1:�Ż����ڶ��н��մ���
		   
** ��  ��: v2.3.6
** �ա���: 2016��7��21��
** �衡��: 1:�޸���ȡ����ʱ�����
           2:���hex_to_bcdת������
		   
** ��  ��: v2.3.5
** �ա���: 2016��6��3��
** �衡��: 1:�޸ķ���Э��汾Ϊ0x01
           2:�̼���������ƫ�����޸�Ϊ4�ֽ�

** ��  ��: v2.3.4
** �ա���: 2016��5��26��
** �衡��: 1:�Ż����ڽ�������
           2:�Ż�������������,ȡ��enum���Ͷ���

** ��  ��: v2.3.3
** �ա���: 2016��5��24��
** �衡��: 1:�޸�mcu��ȡ����ʱ�亯��
           2:���wifi���ܲ���

** ��  ��: v2.3.2
** �ա���: 2016��4��23��
** �衡��: 1:�Ż��������ݽ���
           2:�Ż�MCU�̼���������
           3:�Ż��ϱ�����

** ��  ��: v2.3.1
** �ա���: 2016��4��15��
** �衡��: 1:�Ż��������ݽ���

** ��  ��: v2.3
** �ա���: 2016��4��14��
** �衡��: 1:֧��MCU�̼���������

** ��  ��: v2.2
** �ա���: 2016��4��11��
** �衡��: 1:�޸Ĵ������ݽ��շ�ʽ

** ��  ��: v2.1
** �ա���: 2016��4��8��
** �衡��: 1:����ĳЩ��������֧�ֺ���ָ�����ѡ��

** ��  ��: v2.0
** �ա���: 2016��3��29��
** �衡��: 1:�Ż�����ṹ
           2:��ʡRAM�ռ�
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
                                ��ֲ��֪:
1:MCU������while��ֱ�ӵ���mcu_api.c�ڵ�wifi_uart_service()����
2:����������ʼ����ɺ�,���鲻���йش����ж�,�������ж�,���ж�ʱ������,���жϻ����𴮿����ݰ���ʧ
3:�������ж�/��ʱ���ж��ڵ����ϱ�����
******************************************************************************/

         
/******************************************************************************
                              ��һ��:��ʼ��
1:����Ҫʹ�õ�wifi����ļ����ļ���include "wifi.h"
2:��MCU��ʼ���е���mcu_api.c�ļ��е�wifi_protocol_init()����
3:��MCU���ڵ��ֽڷ��ͺ�������protocol.c�ļ���uart_transmit_output������,��ɾ��#error
4:��MCU���ڽ��պ����е���mcu_api.c�ļ��ڵ�uart_receive_input����,�������յ����ֽ���Ϊ��������
5:��Ƭ������whileѭ�������mcu_api.c�ļ��ڵ�wifi_uart_service()����
******************************************************************************/

/******************************************************************************
                        1:dp���ݵ��������Ͷ��ձ�
          **��Ϊ�Զ����ɴ���,���ڿ���ƽ̨������޸�����������MCU_SDK**         
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
                           2:���ڵ��ֽڷ��ͺ���
�뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
******************************************************************************/

/*****************************************************************************
�������� : uart_transmit_output
�������� : �����ݴ���
������� : value:�����յ��ֽ�����
���ز��� : ��
ʹ��˵�� : �뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
*****************************************************************************/
#include "net_io.h"
void uart_transmit_output(unsigned char value)
{
//  #error "�뽫MCU���ڷ��ͺ�������ú���,��ɾ������"
	
	NET_IO_Send(&value,1);
	
/*
  //ʾ��:
  extern void Uart_PutChar(unsigned char value);
  Uart_PutChar(value);	                                //���ڷ��ͺ���
*/
}
/******************************************************************************
                           �ڶ���:ʵ�־����û�����
1:APP�·����ݴ���
2:�����ϱ�����
******************************************************************************/

/******************************************************************************
                            1:���������ϱ�����
��ǰ��������ȫ�������ϱ�(�������·�/���ϱ���ֻ�ϱ�)
  ��Ҫ�û�����ʵ�����ʵ��:
  1:��Ҫʵ�ֿ��·�/���ϱ����ݵ��ϱ�
  2:��Ҫʵ��ֻ�ϱ����ݵ��ϱ�
�˺���ΪMCU�ڲ��������
�û�Ҳ�ɵ��ô˺���ʵ��ȫ�������ϱ�
******************************************************************************/

//�Զ������������ϱ�����

/*****************************************************************************
�������� : all_data_update
�������� : ϵͳ����dp����Ϣ�ϴ�,ʵ��APP��muc����ͬ��
������� : ��
���ز��� : ��
ʹ��˵�� : �˺���SDK�ڲ������;
           MCU����ʵ�ָú����������ϱ�����;����ֻ�ϱ��Ϳ��ϱ����·�������
*****************************************************************************/
void all_data_update(void)
{
//  #error "���ڴ˴�����·����ϱ����ݼ�ֻ�ϱ�����ʾ��,������ɺ�ɾ������"
	SI7021_measure();
	
  //�˴���Ϊƽ̨�Զ����ɣ��밴��ʵ�������޸�ÿ�����·����ϱ�������ֻ�ϱ�����
  mcu_dp_bool_update(DPID_POWER, download_cmd[0].dp_value); //BOOL�������ϱ�;
  mcu_dp_value_update(DPID_TEMPSET, download_cmd[1].dp_value); //VALUE�������ϱ�;
  mcu_dp_value_update(DPID_TEMPCURRENT,download_cmd[2].dp_value); //VALUE�������ϱ�;
  mcu_dp_enum_update(DPID_MODE,download_cmd[3].dp_value); //ö���������ϱ�;
  mcu_dp_bool_update(DPID_LIGHT,download_cmd[4].dp_value); //BOOL�������ϱ�;
  mcu_dp_fault_update(DPID_FAULT,download_cmd[5].dp_value); //�����������ϱ�;
    
  oled_tuya_dis();

}


/******************************************************************************
                                WARNING!!!    
                            2:���������ϱ�����
�Զ�������ģ�庯��,�������û�����ʵ�����ݴ���
******************************************************************************/


/*****************************************************************************
�������� : dp_download_power_handle
�������� : ���DPID_POWER�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_power_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪBOOL
  unsigned char ret;
  //0:��/1:��
  unsigned char power;
  
  power = mcu_get_dp_download_bool(value,length);
    download_cmd[0].dp_value = power;
    oled_tuya_dis();
  if(power == 0)
  {
    //���ع�
		SI7021_POWER_OFF;
  }
  else
  {
    //���ؿ�
		SI7021_POWER_ON;
  }
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_bool_update(DPID_POWER,power);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_tempset_handle
�������� : ���DPID_TEMPSET�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_tempset_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪVALUE
  unsigned char ret;
  unsigned long tempset;
  
  tempset = mcu_get_dp_download_value(value,length);

  //VALUE�������ݴ���
  download_cmd[1].dp_value = tempset;
  oled_tuya_dis();

  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_value_update(DPID_TEMPSET,tempset);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_mode_handle
�������� : ���DPID_MODE�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_mode_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪENUM
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
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_enum_update(DPID_MODE,mode);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_light_handle
�������� : ���DPID_LIGHT�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_light_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪBOOL
  unsigned char ret;
  //0:��/1:��
  unsigned char light;
  
  light = mcu_get_dp_download_bool(value,length);
	download_cmd[4].dp_value = light;
    oled_tuya_dis();
  if(light == 0)
  {
    //���ع�
		LED2_OFF;
  }
  else
  {
    //���ؿ�
		LED2_ON;
  }
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_bool_update(DPID_LIGHT,light);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}


/******************************************************************************
                                WARNING!!!                     
�˴���ΪSDK�ڲ�����,�밴��ʵ��dp����ʵ�ֺ����ڲ�����
******************************************************************************/
#ifdef SUPPORT_MCU_RTC_CHECK
/*****************************************************************************
�������� : mcu_write_rtctime
�������� : MCUУ�Ա���RTCʱ��
������� : ��
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void mcu_write_rtctime(unsigned char time[])
{
  #error "���������RTCʱ��д�����,��ɾ������"
  /*
  time[0]Ϊ�Ƿ��ȡʱ��ɹ���־��Ϊ 0 ��ʾʧ�ܣ�Ϊ 1��ʾ�ɹ�
  time[1] Ϊ �� �� , 0x00 �� ʾ2000 ��
  time[2]Ϊ�·ݣ��� 1 ��ʼ��12 ����
  time[3]Ϊ���ڣ��� 1 ��ʼ��31 ����
  time[4]Ϊʱ�ӣ��� 0 ��ʼ��23 ����
  time[5]Ϊ���ӣ��� 0 ��ʼ��59 ����
  time[6]Ϊ���ӣ��� 0 ��ʼ��59 ����
  time[7]Ϊ���ڣ��� 1 ��ʼ�� 7 ������1��������һ
 */
  if(time[0] == 1)
  {
    //��ȷ���յ�wifiģ�鷵�صı���ʱ������ 
	 
  }
  else
  {
  	//��ȡ����ʱ�����ݳ���,�п����ǵ�ǰwifiģ��δ����
  }
}
#endif

#ifdef WIFI_TEST_ENABLE
/*****************************************************************************
�������� : wifi_test_result
�������� : wifi���ܲ��Է���
������� : result:wifi���ܲ��Խ��;0:ʧ��/1:�ɹ�
           rssi:���Գɹ���ʾwifi�ź�ǿ��/����ʧ�ܱ�ʾ��������
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void wifi_test_result(unsigned char result,unsigned char rssi)
{
//  #error "������ʵ��wifi���ܲ��Գɹ�/ʧ�ܴ���,��ɺ���ɾ������"
  if(result == 0)
  {
    //����ʧ��
    if(rssi == 0x00)
    {
      //δɨ�赽����Ϊtuya_mdev_test·����,����
    }
    else if(rssi == 0x01)
    {
      //ģ��δ��Ȩ
    }
  }
  else
  {
    //���Գɹ�
    //rssiΪ�ź�ǿ��(0-100, 0�ź���100�ź���ǿ)
  }
  
}
#endif

#ifdef SUPPORT_MCU_FIRM_UPDATE
/*****************************************************************************
�������� : mcu_firm_update_handle
�������� : MCU����̼�����ģʽ
������� : value:�̼�������
           position:��ǰ���ݰ����ڹ̼�λ��
           length:��ǰ�̼�������(�̼�������Ϊ0ʱ,��ʾ�̼����������)
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
//  #error "���������MCU�̼���������,��ɺ���ɾ������"
    unsigned long adder;
    
  if(length == 0)
  {
#ifdef ENABLE_BOOT
    //�̼����ݷ������
    FlashBuffer.magic_code = FIREWARE_UPDATE_FLAG;
    
    STMFLASH_Write(PARA_ADDR,(unsigned int *)&FlashBuffer,(sizeof(FlashBuffer)/4+1));
    
    NVIC_SystemReset();
#endif
    
  }
  else
  {
    //�̼����ݴ���
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
���º����û������޸�!!
******************************************************************************/

/*****************************************************************************
�������� : dp_download_handle
�������� : dp�·�������
������� : dpid:DP���
           value:dp���ݻ�������ַ
           length:dp���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERRO
ʹ��˵�� : �ú����û������޸�
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /*********************************
  ��ǰ����������·�/���ϱ����ݵ���                    
  ���庯������Ҫʵ���·����ݴ���
  �������Ҫ��������������APP��,����APP����Ϊ�·�ʧ��
  ***********************************/
  unsigned char ret;
  switch(dpid)
  {
    case DPID_POWER:
      //���ش�����
      ret = dp_download_power_handle(value,length);
      break;
    case DPID_TEMPSET:
      //Ŀ���¶ȴ�����
      ret = dp_download_tempset_handle(value,length);
      break;
    case DPID_MODE:
      //ģʽ������
      ret = dp_download_mode_handle(value,length);
      break;
    case DPID_LIGHT:
      //�ƹ⴦����
      ret = dp_download_light_handle(value,length);
      break;

  default:
    break;
  }
  return ret;
}
/*****************************************************************************
�������� : get_download_cmd_total
�������� : ��ȡ����dp�����ܺ�
������� : ��
���ز��� : �·������ܺ�
ʹ��˵�� : �ú����û������޸�
*****************************************************************************/
unsigned char get_download_cmd_total(void)
{
  return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}
