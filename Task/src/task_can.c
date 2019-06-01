#include "task_can.h"
#include "can.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"

static bool igbt_en = 0;

void vCanSendTask( void* param )
{
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while ( 1 )
    {
        vTaskDelayUntil( &xLastWakeTime, 50 );
    }
}
void vCanReceiveTask( void* param )
{
    u32         uCanRxID;
    u8          uCanRxBuf[ 8 ];
    static u8   FVoltL, FVoltH, RVoltL, RVoltH;
    static u16  FVolt, RVolt;
    static u8   front_count = 0, rear_count = 0;
    static bool front_valid = 0, rear_valid = 0;
    while ( 1 )
    {

        vTaskDelay( 5 );
        front_count++;
        rear_count++;
        if ( Can_Receive_Msg( uCanRxBuf, &uCanRxID ) )
        {
            switch ( uCanRxID )
            {
            // Front电机反馈信息打包
            case MOTO_F_CANID3:
                FVoltL      = uCanRxBuf[ 0 ];
                FVoltH      = uCanRxBuf[ 1 ];
                front_valid = 1;
                front_count = 0;
                break;
            // Front电机反馈信息打包
            case MOTO_R_CANID3:
                RVoltL     = uCanRxBuf[ 0 ];
                RVoltH     = uCanRxBuf[ 1 ];
                rear_valid = 1;
                rear_count = 0;
                break;
            default:
                break;
            }
        }
        if ( front_count > 200 )
        {
            front_valid = 0;
            front_count = 0;
        }
        if ( rear_count > 200 )
        {
            rear_valid = 0;
            rear_count = 0;
        }
        FVolt = FVoltL + ( FVoltH << 8 );
        RVolt = RVoltL + ( RVoltH << 8 );
        if ( front_valid && rear_valid )
        {
            if ( ( FVolt > PRE_CHARGE_VOLT ) && ( RVolt > PRE_CHARGE_VOLT ) )  //电机驱动器预充完成
            {
                igbt_en = 1;
            }
            else
            {
                igbt_en = 0;
            }
        }
        else if ( front_valid && ( !rear_valid ) )
        {
            if ( FVolt > PRE_CHARGE_VOLT )  //电机驱动器预充完成
            {
                igbt_en = 1;
            }
            else
            {
                igbt_en = 0;
            }
        }
        else if ( ( !front_valid ) && rear_valid )
        {
            if ( RVolt > PRE_CHARGE_VOLT )  //电机驱动器预充完成
            {
                igbt_en = 1;
            }
            else
            {
                igbt_en = 0;
            }
        }
        else
        {
            igbt_en = 0;
        }
    }
}

bool getIGBTEn( void )
{
    return igbt_en;
}
