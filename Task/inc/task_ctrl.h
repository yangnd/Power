#ifndef __TASK_CTRL_H
#define __TASK_CTRL_H
#include "sys.h"

//#define THD_ON GPIO_SetBits(GPIOF,GPIO_Pin_6)
//#define CAP_ON GPIO_SetBits(GPIOF,GPIO_Pin_8)
#define MP_IGBT PFout( 0 )
#define CAP_IGBT PFout( 2 )
#define BAT_IGBT PFout( 4 )
#define CHARGE_CAP PFout( 1 )
#define CHARGE_BAT PFout( 3 )
#define BAT_TO_CAP PFout( 5 )

void vChargeCtrlTask( void* param );
void vIGBTCtrlTask( void* param );

#endif
