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

    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);

    while(1)
    {
        if(r->Button[0])
        {
            
        }
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 50);
        osDelayUntil(&time, 1000);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 250);
			osDelayUntil(&time, 1000);
    }
}