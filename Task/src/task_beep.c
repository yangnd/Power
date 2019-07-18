#include "task_beep.h"
#include "beep.h"
#include "task_sensor.h"
#include "task_ctrl.h"

/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
void vBeepTask( void* param )
{
    static u8 uSensorState;
	vTaskDelay(5000);
    while ( 1 )
    {
        vTaskDelay( 300 );
        BEEP = 0;
        vTaskDelay( 300 );
        uSensorState = getSensorState();
		if(getErrorMode())
		{
				BEEP=1;
		}
		else
		{
			//		uSensorState=uSensorState&0x3F;	//强制bit6：0
			if ( uSensorState >> 4 )
				BEEP = 1;
		}
        
    }
}
