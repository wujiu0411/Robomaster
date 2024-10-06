#include "main.h"
#include "cmsis_os.h"
#include <tim.h>
#include "ble_remote.h"
#include "usart.h"


void StartSevorTask(void const * argument)
{
    uint32_t time = xTaskGetTickCount();
    extern uint8_t data[11];
    const remote_t *r=get_remote_control_point();
    int Angle=0;
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    
    while(1)
    {
        if(r->Button[0])
        {
            __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 45);
            osDelay(1000);
        }
        if(r->Button[1])
        {   
            __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 180/180*2000+500);
             osDelay(1000);
        }
        if(r->Button[2])
        {
            HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_SET);//控制继电器开，启动风机
            osDelay(1000);
        }

        if(r->Button[3])
        {   
            HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_RESET);//控制继电器关，关闭风机
            osDelay(1000);
        }
        
        if(r->Switch[0])
        {
            __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 220);//球门开
            osDelay(1000);
        }
        if(r->Switch[1])
        {   
            __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 260);//球门关
             osDelay(1000);
        }
       __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 220);
        HAL_Delay(1000);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2,260);
        HAL_Delay(1000);
    }
		     osDelayUntil(&time,1);
}