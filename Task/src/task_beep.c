#include "task_beep.h"
#include "task_ctrl.h"
#include "beep.h"
#include <stdbool.h>
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
static bool igbtErr;
void vBeepTask(void *param)
{
	while (1)
	{
		vTaskDelay(300);
		BEEP = 0;
		vTaskDelay(300);
		igbtErr = getIGBTErr();
		if (igbtErr)
			BEEP = 1;
	}
}
