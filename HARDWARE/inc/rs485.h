#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  
//////////////////////////////////////////////////////////////////////////////////	 
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	  		  	
extern u8 RS485_RX_BUF[64]; 		//接收缓冲,最大64个字节
extern u8 RS485_RX_CNT;   			//接收到的数据长度

//模式控制
#define RS485_TX_EN		PDout(7)	//485模式控制.0,接收;1,发送.
//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART2_RX 	1			//0,不接收;1,接收.




void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
u8 RS485_Receive_Data(u8 *buf,u8 *len);
u16 crc_chk(u8 *data,u8 length);
void ModbusWriteSReg(u8 *txbuf, u8 txlen);
/*设置RS485中断回调函数*/
void rs485_setIterruptCallback(void (*cb)(void));

#endif
