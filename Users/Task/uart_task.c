#include "main.h"
#include "cmsis_os.h"
#include "ble_remote.h"
#include "usart.h"
#include <stdio.h>

void StartUARTTask(void const * argument)
{
    uint32_t time = xTaskGetTickCount();
    extern uint8_t data[11];
    const remote_t *r=get_remote_control_point();
    float xPosition = r->rocker[0].x_position;
    float yPosition = r->rocker[0].y_position;  
    if (r->Button[0]) {  
        // 如果第一个按钮被按下（假设Button[0]为1表示按下）  
        // 执行一些操作  
    }  
    
  
    while (1)
    {
        //memset(data,0,sizeof(data));
        //HAL_UART_Receive_DMA(&huart3,data,11);
        osDelayUntil(&time, 1);
    }

}