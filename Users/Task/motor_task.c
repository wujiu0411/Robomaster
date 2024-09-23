#include "main.h"
#include "cmsis_os.h"
#include "motor.h"
#include "pid.h"
#include "usart.h"
#include "ble_remote.h"

void StartmotorTask(void const * argument)
{
    uint32_t time = xTaskGetTickCount();
    extern uint8_t data[11];
    const remote_t *r=get_remote_control_point();
    float xPosition = r->rocker[0].x_position;
    float yPosition = r->rocker[0].y_position;  
    Motor motor_a,motor_b,motor_c,motor_d;

    HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim5,TIM_CHANNEL_ALL);

    Motor_Init(&motor_a, &htim2, &htim8, TIM_CHANNEL_1, AIN1_GPIO_Port, AIN1_Pin, AIN2_GPIO_Port, AIN2_Pin);
    Motor_Init(&motor_b, &htim3, &htim8, TIM_CHANNEL_2, BIN1_GPIO_Port, BIN1_Pin, BIN2_GPIO_Port, BIN2_Pin);
    Motor_Init(&motor_c, &htim4, &htim8, TIM_CHANNEL_3, CIN1_GPIO_Port, CIN1_Pin, CIN2_GPIO_Port, CIN2_Pin);
    Motor_Init(&motor_d, &htim5, &htim8, TIM_CHANNEL_4, DIN1_GPIO_Port, DIN1_Pin, DIN2_GPIO_Port, DIN2_Pin);

    motor_a.PidInit(&motor_a, PID_DELTA, 7200,100,4.5,0.4,0.4);
    motor_b.PidInit(&motor_b, PID_DELTA, 7200,100,4.5,0.4,0.4);
    motor_c.PidInit(&motor_c, PID_DELTA, 7200,100,4.5,0.4,0.4);
    motor_d.PidInit(&motor_d, PID_DELTA, 7200,100,4.5,0.4,0.4);

    __HAL_TIM_SetCounter(&htim2,10000);
    __HAL_TIM_SetCounter(&htim3,10000);
    __HAL_TIM_SetCounter(&htim4,10000);
    __HAL_TIM_SetCounter(&htim5,10000);
    
    motor_a.speed_set=xPosition-yPosition;
    motor_b.speed_set=xPosition+yPosition;
    motor_c.speed_set=-xPosition+yPosition;
    motor_d.speed_set=-xPosition+yPosition;

    while(1)
    {
        motor_a.Driver(&motor_a, motor_a.speed);
        motor_b.Driver(&motor_b, motor_b.speed);
        motor_c.Driver(&motor_c, motor_c.speed);
        motor_d.Driver(&motor_d, motor_d.speed);

        osDelayUntil(&time, 1);
    }

}