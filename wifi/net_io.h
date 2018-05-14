#ifndef _NET_IO_H_
#define _NET_IO_H_



extern unsigned char wifi_state;

#define RB_BUFFER_SIZE		256

#define RB_BUFFER2_SIZE		256


typedef struct
{

/*************************ѭ������*************************/
	unsigned char buf[RB_BUFFER_SIZE];		//���ջ���
	unsigned char buf_tp[RB_BUFFER2_SIZE];	//�������棬һ֡���ݳ�����rbĩβ���׵�ʱ�򣬰����ݸ��Ƶ��������棬�Է����ϲ����������
	unsigned short write_pos;				//д
	unsigned short read_pos;				//��
	unsigned short data_packet;				//���ݰ�

} NET_IO_INFO;

/*************************��Ҫ�޸ĵĵط�*************************/
#define RCC_IO_WIFI				RCC_AHB1Periph_GPIOD
#define PIN_WIFI				GPIO_Pin_8 | GPIO_Pin_9
#define GPIO_WIFI				GPIOD
#define AF_WIFI_TX				GPIO_PinSource8
#define AF_WIFI_RX				GPIO_PinSource9

#define WIFI_IRQn				USART3_IRQn

#define RCC_USART_WIFI			RCC_APB1Periph_USART3
#define	BAUDRATE_WIFI			9600
#define NET_IO					USART3
/*************************��Ҫ�޸ĵĵط�*************************/

void NET_Timer_Init(void);

void NET_USART_Init(void);

void NET_IO_Init(void);

void NET_IO_Send(unsigned char *str, unsigned short len);

unsigned char *NET_IO_Read(void);


#endif
