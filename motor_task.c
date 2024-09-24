#include "main.h"
#include "cmsis_os.h"
#include "motor.h"
#include "pid.h"
#include "usart.h"
#include "ble_remote.h"

void StartmotorTask(void const *argument)
{
    uint32_t time = xTaskGetTickCount();
    float Vx, Vy, Vz, k;
    int16_t run[4];
    int16_t chassis[4];
    int16_t feedback[4];
    const remote_t *r = get_remote_control_point();
    for (int i = 0; i < 4; i++)
    {
        feedback[i] = 0;
        chassis[i] = 0;
        run[i] = 0;
        Motor motor1, motor2, motor3, motor4;

        HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
        HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
        HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
        HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);

        Motor_Init(&motor1, &htim2, &htim8, TIM_CHANNEL_1, AIN1_GPIO_Port, AIN1_Pin, AIN2_GPIO_Port, AIN2_Pin);
        Motor_Init(&motor2, &htim3, &htim8, TIM_CHANNEL_2, BIN1_GPIO_Port, BIN1_Pin, BIN2_GPIO_Port, BIN2_Pin);
        Motor_Init(&motor3, &htim4, &htim8, TIM_CHANNEL_3, CIN1_GPIO_Port, CIN1_Pin, CIN2_GPIO_Port, CIN2_Pin);
        Motor_Init(&motor4, &htim5, &htim8, TIM_CHANNEL_4, DIN1_GPIO_Port, DIN1_Pin, DIN2_GPIO_Port, DIN2_Pin);

        motor1.PidInit(&motor1, PID_DELTA, 7200, 100, 4.5, 0.4, 0.4);
        motor2.PidInit(&motor2, PID_DELTA, 7200, 100, 4.5, 0.4, 0.4);
        motor3.PidInit(&motor3, PID_DELTA, 7200, 100, 4.5, 0.4, 0.4);
        motor4.PidInit(&motor4, PID_DELTA, 7200, 100, 4.5, 0.4, 0.4);

        __HAL_TIM_SetCounter(&htim2, 10000);
        __HAL_TIM_SetCounter(&htim3, 10000);
        __HAL_TIM_SetCounter(&htim4, 10000);
        __HAL_TIM_SetCounter(&htim5, 10000);

        while (1)
        {
            Vx = r->rocker[0].x_position;
            Vy = r->rocker[0].y_position;
            Vz = r->rocker[0].angle;
            chassis[0] = Vx + Vy - Vz * k; // k是一个参数 达到最丝滑旋转效果就好
            chassis[1] = Vx - Vy - Vz * k;
            chassis[2] = Vx + Vy + Vz * k;
            chassis[3] = Vx - Vy + Vz * k;
            motor1.EncoderUpdate(&motor1);
            osDelay(1);
            motor1.SpeedGet(&motor1);
            feedback[0] = motor1.speed;
            motor2.EncoderUpdate(&motor2);
            osDelay(1);
            motor2.SpeedGet(&motor2);
            feedback[1] = motor2.speed;
            motor3.EncoderUpdate(&motor3);
            osDelay(1);
            motor3.SpeedGet(&motor3);
            feedback[2] = motor3.speed;
            motor4.EncoderUpdate(&motor4);
            osDelay(1);
            motor4.SpeedGet(&motor4);
            feedback[3] = motor4.speed;
            run[0] = PID_Calc(&motor1, feedback[0], chassis[0]);
            run[1] = PID_Calc(&motor2, feedback[1], chassis[1]);
            run[2] = PID_Calc(&motor3, feedback[2], chassis[2]);
            run[3] = PID_Calc(&motor4, feedback[3], chassis[3]);
            motor1.Driver(&motor1, run[0]);
            motor2.Driver(&motor2, run[1]);
            motor3.Driver(&motor3, run[2]);
            motor4.Driver(&motor4, run[3]);

            osDelayUntil(&time, 1);
        }
    }