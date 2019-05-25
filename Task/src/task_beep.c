#include "task_beep.h"
#include "task_sensor.h"
#include "beep.h"

/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
void vBeepTask(void *param)
{
	u8 uSensorState;
	while (1)
	{
		vTaskDelay(300);
		BEEP = 0;
		vTaskDelay(300);
		uSensorState=getSensorState();
		uSensorState=uSensorState&0x3F;	//强制bit6：0
		if (uSensorState>>4)
			BEEP = 1;
	}
}
