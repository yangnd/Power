#include "sensor.h"

void Power_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF, ENABLE); 				//使能PE,PF端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9
					| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13; //端口配置7~13
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                    	//输入下拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);                              //根据设定参数初始化GPIOE.7~13
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;	//端口配置0~5
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                    //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;                   //IO口速度为10MHz
    GPIO_Init(GPIOF, &GPIO_InitStructure);                              //根据设定参数初始化GPIOF.11/13/15
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);									//输出口复位
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);
	GPIO_ResetBits(GPIOF,GPIO_Pin_3);
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);
}
