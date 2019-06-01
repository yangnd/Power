#ifndef __TASK_SENSOR_H
#define __TASK_SENSOR_H
#include "sys.h"

#define MP_VOLT_N GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_7 )      //第三轨可用
#define CAP_VOLT_N GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_9 )     //超级电容可用
#define MP_CURR_H GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_11 )     //第三轨电流高
#define CAP_CURR_H GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_13 )    //超级电容电流高
#define MP_IGBT_ERR GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_8 )    //第三轨IGBT Error
#define CAP_IGBT_ERR GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_10 )  //第三轨IGBT Error
#define BAT_IGBT_ERR GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_12 )  //第三轨IGBT Error

void vSensorTask( void* param );
u8   getSensorState( void );
#endif
