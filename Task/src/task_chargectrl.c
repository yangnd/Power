#include "task_chargectrl.h"
#include "task_sensor.h"
#include <stdbool.h>
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"

static bool MV_N=0,BATV_N=1;
static u8 uSensorState=0;

