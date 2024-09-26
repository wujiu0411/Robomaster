#include "main.h"
#include "cmsis_os.h"
#include "motor.h"
#include "pid.h"
#include "usart.h"
#include "ble_remote.h"

#define tread 2.65 //轮距
#define wheelbase 2.20 //轴距

/**
 * @brief 编码器初始值置回10000，用于速度计算
 * @author Hujunhao
 * @date 2024-9-23
 * @retval None
 */
void reset_counter()
{ 
    __HAL_TIM_SetCounter(&htim2,10000);
    __HAL_TIM_SetCounter(&htim3,10000);
    __HAL_TIM_SetCounter(&htim4,10000);
    __HAL_TIM_SetCounter(&htim5,10000);
}


void StartmotorTask(void const * argument)
{
    uint32_t time = xTaskGetTickCount();
    const remote_t *r=get_remote_control_point();

    float Vx = r->rocker[0].x_position;
    float Vy = r->rocker[0].y_position;  
    float Vz = r->rocker[0].angle;
    float k=Vz*(tread/2+wheelbase/2);

    Motor motor[4];

    Motor_Init(&motor[0], &htim2, &htim8, TIM_CHANNEL_1, AIN1_GPIO_Port, AIN1_Pin, AIN2_GPIO_Port, AIN2_Pin);
    Motor_Init(&motor[1], &htim3, &htim8, TIM_CHANNEL_2, BIN1_GPIO_Port, BIN1_Pin, BIN2_GPIO_Port, BIN2_Pin);
    Motor_Init(&motor[2], &htim4, &htim8, TIM_CHANNEL_3, CIN1_GPIO_Port, CIN1_Pin, CIN2_GPIO_Port, CIN2_Pin);
    Motor_Init(&motor[3], &htim5, &htim8, TIM_CHANNEL_4, DIN1_GPIO_Port, DIN1_Pin, DIN2_GPIO_Port, DIN2_Pin);

    motor[0].PidInit(&motor[0], PID_DELTA, 20000-1,100,4.5,0.4,0.4);
    motor[1].PidInit(&motor[1], PID_DELTA, 20000-1,100,4.5,0.4,0.4);
    motor[2].PidInit(&motor[2], PID_DELTA, 20000-1,100,4.5,0.4,0.4);
    motor[3].PidInit(&motor[3], PID_DELTA, 20000-1,100,4.5,0.4,0.4);

    __HAL_TIM_SetCounter(&htim2,10000);
    __HAL_TIM_SetCounter(&htim3,10000);
    __HAL_TIM_SetCounter(&htim4,10000);
    __HAL_TIM_SetCounter(&htim5,10000);
    

    while(1)
    {
      Vx = r->rocker[0].x_position;
      Vy = r->rocker[0].y_position;
      Vz = r->rocker[0].angle;
      k=Vz*(tread/2+wheelbase/2);

      motor[0].speed_set = Vx + Vy - Vz * k;
      motor[1].speed_set = Vx - Vy - Vz * k;
      motor[2].speed_set = Vx + Vy + Vz * k;
      motor[3].speed_set = Vx - Vy + Vz * k;

     for (size_t i = 0; i < 4; i++) 
      {
        motor[i].Driver(&motor[i], motor[i].speed);
        motor[i].EncoderUpdate(&motor[i]);
        motor[i].SpeedGet(&motor[i]);
        motor[i].Calc(&motor[i]);
        motor[i].speed += motor[i].pid.out;
        
        osDelayUntil(&time, POLL_INTERVAL_MS);
      }

      osDelayUntil(&time,500);

    }

}