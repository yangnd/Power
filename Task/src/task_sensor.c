#include "task_sensor.h"
#include <stdbool.h>
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"

static u8 uMpVolt,uCapVolt,uMainCurr,uCapCurr,uMpIGBTErr,uCapIGBTErr,uBatIGBTErr;

static u8 uSensorState=0x00;
void vSensorTask(void *param)
{
	int i;
	while(1)
	{
		vTaskDelay(20);
		uMpVolt=0;
		uCapVolt=0;
		uMainCurr=0;
		uCapCurr=0;
		uMpIGBTErr=0;
		uCapIGBTErr=0;
		uBatIGBTErr=0;
		for(i=0;i<10;i++)
		{
			vTaskDelay(2);
			uMpVolt=uMpVolt+MP_VOLT_N;
			uCapVolt=uCapVolt+CAP_VOLT_N;
			uMainCurr=uMainCurr+MP_CURR_H;
			uCapCurr=uCapCurr+CAP_CURR_H;
			uMpIGBTErr=uMpIGBTErr+MP_IGBT_ERR;
			uCapIGBTErr=uCapIGBTErr+CAP_IGBT_ERR;
			uBatIGBTErr=uBatIGBTErr+BAT_IGBT_ERR;
		}
		//BIT0
		if(uMpVolt>7) uSensorState=uSensorState|0x01;
		else if(uMpVolt<3) uSensorState=uSensorState&0xFE;
		//BIT1
		if(uCapVolt>7) uSensorState=uSensorState|0x02;
		else if(uCapVolt<3) uSensorState=uSensorState&0xFD;
		//BIT2
		if(uMainCurr>7) uSensorState=uSensorState|0x04;
		else if(uMainCurr<3) uSensorState=uSensorState&0xFB;
		//BIT3
		if(uCapCurr>7) uSensorState=uSensorState|0x08;
		else if(uCapCurr<3) uSensorState=uSensorState&0xF7;
		//BIT4
		if(uMpIGBTErr<3) uSensorState=uSensorState|0x10;		//低电平为Err
		else if(uMpIGBTErr>7) uSensorState=uSensorState&0xEF;
		//BIT5
		if(uCapIGBTErr<3) uSensorState=uSensorState|0x20;		//低电平为Err
		else if(uCapIGBTErr>7) uSensorState=uSensorState&0xDF;
		//BIT6
		if(uBatIGBTErr<3) uSensorState=uSensorState|0x40;		//低电平为Err
		else if(uBatIGBTErr>7) uSensorState=uSensorState&0xBF;
	}
}
u8 getSensorState(void)
{
	return uSensorState;
}
