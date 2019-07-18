#include "task_ctrl.h"
#include "led.h"
#include "task_can.h"
#include "task_key.h"
#include "task_readbat.h"
#include "task_sensor.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"

static u8   uKeyState;
bool        mp_manual = 0, mp_auto = 0, cap_manual = 0, cap_auto = 0, bat_manual = 0, bat_auto = 0, charge_cap = 0, charge_bat = 0, bat_to_cap = 0, ctrl_mode = 0;  // mode:0 manual,1 auto
bool        mpVoltN, capVoltN, mpCurrH, capCurrH, mpIGBTErr, capIGBTErr, batIGBTErr;
static bool error_mode=0,error_reset=0;
static u8   batStatus;
static u8   uSensorState = 0;
static bool igbt_en      = 0;

void vIGBTCtrlTask( void* param )
{
    static bool mode_flag = 0;
    vTaskDelay( 2000 );  //等待传感器状态
    while ( 1 )
    {
        vTaskDelay( 20 );
        igbt_en = getIGBTEn();
        //		igbt_en=1;	//调试强制
        if ( igbt_en )
            LED1 = 0;
        else
            LED1 = 1;
        uKeyState = getKeyState();
        switch ( uKeyState )
        {
        case KEY0_SHORT_PRESS:
            // RESV
            break;
        case KEY0_MID_PRESS:
            charge_cap = !charge_cap;
            break;
        case KEY0_LONG_PRESS:
            mp_manual = !mp_manual;
            break;
        case KEY1_SHORT_PRESS:
            // RESV
            break;
        case KEY1_MID_PRESS:
            charge_bat = !charge_bat;
            break;
        case KEY1_LONG_PRESS:
            cap_manual = !cap_manual;
            break;
        case WKUP_SHORT_PRESS:
            error_reset=1;
            break;
        case WKUP_MID_PRESS:
            ctrl_mode = !ctrl_mode;
            break;
        case WKUP_LONG_PRESS:
            bat_manual = !bat_manual;
            break;
        default:
            break;
        }
		// SensorState
		uSensorState = getSensorState();
		mpVoltN    = uSensorState & 0x01;
		capVoltN   = ( uSensorState & 0x02 ) >> 1;
		mpCurrH    = ( uSensorState & 0x04 ) >> 2;
		capCurrH   = ( uSensorState & 0x08 ) >> 3;
		mpIGBTErr  = ( uSensorState & 0x10 ) >> 4;
		capIGBTErr = ( uSensorState & 0x20 ) >> 5;
		batIGBTErr = ( uSensorState & 0x40 ) >> 6;
        /*手动模式*/
        if ( !ctrl_mode )
        {
            LED0 = 1;
            if ( mode_flag )
            {
                mode_flag = 0;
                //自动切手动，保持切换前状态
                mp_manual  = MP_IGBT;
                cap_manual = CAP_IGBT;
                bat_manual = BAT_IGBT;
            }
			if ( mpIGBTErr || capIGBTErr || batIGBTErr )
			{
				error_mode=1;
				if ( mpIGBTErr )
					mp_manual = 0;
				if ( capIGBTErr )
					cap_manual = 0;
				if ( batIGBTErr )
					bat_manual = 0;
			}
			else
			{
				if(error_reset)
				{
					error_mode=0;
					error_reset=0;
				}
			}
            // IGBT控制
            if ( !igbt_en )
            {
                bat_manual = 0;
            }
            MP_IGBT  = mp_manual;
            CAP_IGBT = cap_manual;
            BAT_IGBT = bat_manual;
        }
        /*自动模式*/
        else
        {
			error_mode=0;
            LED0 = 0;
            if ( !mode_flag )
            {
                mode_flag = 1;
            }
            if ( igbt_en )
            {
                // IGBT
                if ( mpIGBTErr || capIGBTErr || batIGBTErr )
                {
                    if ( mpIGBTErr )
                        MP_IGBT = 0;
                    if ( capIGBTErr )
                        CAP_IGBT = 0;
                    if ( batIGBTErr )
                        BAT_IGBT = 0;
                }
                else
                {
                    if ( mpVoltN )
                    {
                        if ( BAT_IGBT )
                        {
                            MP_IGBT = 1;
                            vTaskDelay( 500 );
                            BAT_IGBT = 0;
                        }
                        if ( ( !capVoltN ) || ( !capCurrH ) )
                        {
                            MP_IGBT = 1;
                        }
                        else
                        {
                            vTaskDelay( 500 );
                            if ( CAP_IGBT == 1 )  //防止电容断开时，电容电流高虚假信号使第三轨错误断开
                                MP_IGBT = 0;
                        }

                        if ( !capVoltN )
                        {
                            MP_IGBT = 1;
                            vTaskDelay( 500 );
                            CAP_IGBT = 0;
                        }
                        else
                        {
                            if ( mpCurrH )
                                CAP_IGBT = 1;
                        }
                    }
                    else
                    {
                        BAT_IGBT = 1;
                        CAP_IGBT = 1;
                        vTaskDelay( 500 );
                        MP_IGBT = 0;
                    }
                }
            }
            else
            {
                MP_IGBT  = 0;
                CAP_IGBT = 0;
                BAT_IGBT = 0;
            }
        }
    }
}
void vChargeCtrlTask( void* param )
{
    /*
    第三轨正常：
        超级电容一直充电，
        当电池电量低时，给电池充电，停止超级电容充电
    第三轨故障：
        电池给超级电容充电
    */
    static bool mode_flag = 0;
    vTaskDelay( 2000 );  //等待传感器状态
    while ( 1 )
    {
        vTaskDelay( 500 );
        batStatus = getBatStatus();
        /*手动模式*/
        if ( !ctrl_mode )
        {
            if ( mode_flag )
            {
                mode_flag = 0;
                //自动切手动，保持切换前状态
                charge_cap = CHARGE_CAP;
                charge_bat = CHARGE_BAT;
            }
            CHARGE_CAP = charge_cap;
            CHARGE_BAT = charge_bat;
        }
        /*自动模式*/
        else
        {
            if ( !mode_flag )
            {
                mode_flag = 1;
            }
            if ( mpVoltN )  //第三轨正常
            {
                if ( batStatus == BAT_CHG_START )
                {
                    CHARGE_CAP = 0;
                    vTaskDelay( 100 );
                    CHARGE_BAT = 1;
                }
                else
                {
                    CHARGE_BAT = 0;
                    vTaskDelay( 100 );
                    CHARGE_CAP = 1;
                }
                BAT_TO_CAP = 0;
            }
            else  //第三轨故障
            {
                CHARGE_BAT = 0;
                BAT_TO_CAP = 1;
                vTaskDelay( 100 );
                CHARGE_CAP = 1;
            }
        }
    }
}

bool getErrorMode(void)
{
	return error_mode;
}
