/**
 ********************************  STM32F10x  *********************************
 * @文件名     ： main.c
 * @作者       ：
 * @库版本     ： V3.5.0
 * @文件版本   ： V1.0.0
 * @日期       ： 2019年03月20日
 * @摘要       ： 主函数
 ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2019-03-20 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
/* Standard includes. */
//#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
//#include "queue.h"

/* Library includes. */
#include "stm32f10x_it.h"

/* app includes. */
#include "task_beep.h"
#include "task_can.h"
#include "task_ctrl.h"
#include "task_key.h"
#include "task_readbat.h"
#include "task_sensor.h"

#include "beep.h"
#include "can.h"
#include "delay.h"
#include "key.h"
#include "led.h"
#include "sensor.h"

static TaskHandle_t startTaskHandle;
static void         startTask( void* param );

/************************************************
函数名称 ： main
功    能 ： 主函数入口
参    数 ： 无
返 回 值 ： int
作    者 ：
*************************************************/
int main( void )
{
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 ); /*中断配置初始化*/
    delay_init();
    KEY_Init();  //按键初始化
    LED_Init();  // LED灯初始化
    BEEP_Init();
    Power_Init();
    Modbus_Init();
    CAN_Mode_Init( CAN_SJW_1tq, CAN_BS2_8tq, CAN_BS1_9tq, 8, CAN_Mode_Normal );  // CAN初始化正常模式,波特率250Kbps
    delay_ms( 3000 );
    xTaskCreate( startTask, "START_TASK", 100, NULL, 2, &startTaskHandle ); /*创建起始任务*/
    vTaskStartScheduler();                                                  /*开启任务调度*/

    while ( 1 )
    {
    }; /* 任务调度后不会执行到这 */
}

/*创建任务*/
void startTask( void* param )
{
    taskENTER_CRITICAL();                                             /*进入临界区*/
    xTaskCreate( vIGBTCtrlTask, "IGBTCtrl", 100, NULL, 3, NULL );     /*创建IGBT控制任务*/
    xTaskCreate( vChargeCtrlTask, "ChargeCtrl", 100, NULL, 3, NULL ); /*创建charge控制任务*/
    xTaskCreate( vCanReceiveTask, "CanReceive", 100, NULL, 2, NULL ); /*创建can接收任务*/
    xTaskCreate( vSensorTask, "Sensor", 100, NULL, 2, NULL );         /*创建sensor读取任务*/
    xTaskCreate( vReadBatTask, "ReadBat", 100, NULL, 2, NULL );       /*创建bat读取任务*/
    xTaskCreate( vKeyTask, "Key", 100, NULL, 1, NULL );               /*创建按键扫描任务*/
    xTaskCreate( vBeepTask, "Beep", 100, NULL, 1, NULL );             /*创建beep任务*/

    vTaskDelete( startTaskHandle ); /* 删除开始任务 */

    taskEXIT_CRITICAL(); /*退出临界区*/
}

/**** Copyright (C)2019 FeiSuo. All Rights Reserved **** END OF FILE ****/
