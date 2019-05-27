#include "task_readbat.h"
#include "rs485.h"

/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

static xSemaphoreHandle rs485rxIT;
static u8 batStatus;

/*RS485外部中断回调函数*/
static void rs485_interruptCallback(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(rs485rxIT, &xHigherPriorityTaskWoken);
//	portYIELD_FROM_ISR(xHigherPriorityTaskWoken); //如果需要的话进行一次任务切换
}
void Modbus_Init(void)
{
	RS485_Init(9600);
	rs485rxIT = xSemaphoreCreateBinary();
	rs485_setIterruptCallback(rs485_interruptCallback);
}
u8 ModbusReadSReg(u8 id,u16 addr,u16 *data)
{
	modbusCmd_t readData;
	u16 rx_crc,cal_crc;
	u8 ackStatus;
	portBASE_TYPE state;
	readData.id=id;
	readData.cmd=0x03;	//read cmd
	readData.addrH=addr>>8;
	readData.addrL=addr&0x00FF;
	readData.lenH=0x00;
	readData.lenL=0x01;
	readData.crc=crc_chk((u8*)&readData,6);
	RS485_Send_Data((u8*)&readData,8);	
	state=xSemaphoreTake(rs485rxIT, 100);
	if(state==pdTRUE)
	{
		if(RS485_RX_CNT==7)
		{
			cal_crc=crc_chk(RS485_RX_BUF,RS485_RX_CNT-2);
			rx_crc=((u16)RS485_RX_BUF[RS485_RX_CNT-1]<<8)+RS485_RX_BUF[RS485_RX_CNT-2];
			if(cal_crc==rx_crc)
			{
				*data=(RS485_RX_BUF[3]<<8)+RS485_RX_BUF[4];
				ackStatus= MODBUS_ACK_OK;
			}
			else
			{
				//校验错误
				ackStatus= MODBUS_ACK_CRC_ERR;
			}
		}
		else if(RS485_RX_CNT==5)
		{
			ackStatus=MODBUS_ACK_NOTOK;
		}
		else
		{
			ackStatus=MODBUS_ACK_FRAME_ERR;
		}		
	}
	else
	{
		//超时
		ackStatus= MODBUS_ACK_TIMEOUT;
	}
	return ackStatus;
}
void vReadBatTask(void *param)
{
	u8 ackStatus;
	u16 batPercent;
	while(1)
	{
		vTaskDelay(200);
		ackStatus=ModbusReadSReg(0x03,0x03,&batPercent);	//id:03 address:03
		if(ackStatus==MODBUS_ACK_OK)
		{
			if(batPercent>95)
			{
				batStatus=BAT_CHG_COMPTETE;
			}
			else if(batPercent<50)
			{
				batStatus=BAT_CHG_START;
			}
			else
			{
				batStatus=batStatus;
			}
		}
		else
		{
			batStatus=BAT_COMM_ERR;
		}
	}
}

u8 getBatStatus(void)
{
	return batStatus;
}
