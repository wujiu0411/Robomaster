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
  int16_t feedback[4];
  k = 0.265 / 2 + 0.220 / 2;
  const remote_t *r = get_remote_control_point();
  for (int i = 0; i < 4; i++)
  {
    feedback[i] = 0;
    run[i] = 0;
  }
  Motor motor[4];

  Motor_Init(&motor[0], &htim2, &htim8, TIM_CHANNEL_1, AIN1_GPIO_Port, AIN1_Pin, AIN2_GPIO_Port, AIN2_Pin);
  Motor_Init(&motor[1], &htim3, &htim8, TIM_CHANNEL_2, BIN1_GPIO_Port, BIN1_Pin, BIN2_GPIO_Port, BIN2_Pin);
  Motor_Init(&motor[2], &htim4, &htim8, TIM_CHANNEL_3, CIN1_GPIO_Port, CIN1_Pin, CIN2_GPIO_Port, CIN2_Pin);
  Motor_Init(&motor[3], &htim5, &htim8, TIM_CHANNEL_4, DIN1_GPIO_Port, DIN1_Pin, DIN2_GPIO_Port, DIN2_Pin);

  for (size_t i = 0; i < 4; i++)
  {
    motor[i].PidInit(&motor[i], PID_DELTA, 7200, 100, 4.5, 0.4, 0.4);
  }

  // __HAL_TIM_SetCounter(&htim2, 10000);
  // __HAL_TIM_SetCounter(&htim3, 10000);
  // __HAL_TIM_SetCounter(&htim4, 10000);
  // __HAL_TIM_SetCounter(&htim5, 10000);

  while (1)
  {
    Vx = r->rocker[0].x_position;
    Vy = r->rocker[0].y_position;
    Vz = r->rocker[0].angle;
    motor[0].speed_set = Vx + Vy - Vz * k; // k是一个参数 达到最丝滑旋转效果就好
    motor[1].speed_set = Vx - Vy - Vz * k;
    motor[2].speed_set = Vx + Vy + Vz * k;
    motor[3].speed_set = Vx - Vy + Vz * k;
    for (size_t i = 0; i < 4; i++)
    {
      motor[i].EncoderUpdate(&motor[i]);
    }
    osDelay(1);
    for (size_t i = 0; i < 4; i++)
    {
      motor[i].SpeedGet(&motor[i]);
      // feedback[i] = motor[i].speed;
    }

    for (size_t i = 0; i < 4; i++)
    {
      motor[i].Calc(&motor[i]);
      // motor[i].Driver(&motor[i], motor[i].pid.out);
      motor[i].Driver(&motor[i], 7000);
      // run[i] = PID_Calc(&motor[i].pid, feedback[i], chassis[i]);
      // motor[i].Driver(&motor[i], 16000);
    }
    osDelayUntil(&time, 99);
  }
}