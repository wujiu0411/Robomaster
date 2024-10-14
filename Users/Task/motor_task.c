#include "main.h"
#include "cmsis_os.h"
#include "motor.h"
#include "pid.h"
#include "usart.h"
#include "ble_remote.h"
#include "math.h"

#define tread 0.265 //轮距
#define wheelbase 0.220 //轴距

void StartmotorTask(void const *argument)
{
  uint32_t time = xTaskGetTickCount();
  float Vx, Vy, Vz, Va;
  float k = 0.265 / 2 + 0.220 / 2;
  float m=2;
  const remote_t *r = get_remote_control_point();
  Motor motor[4];

  Motor_Init(&motor[0], &htim2, &htim8, TIM_CHANNEL_1, AIN1_GPIO_Port, AIN1_Pin, AIN2_GPIO_Port, AIN2_Pin);
  Motor_Init(&motor[1], &htim3, &htim8, TIM_CHANNEL_2, BIN1_GPIO_Port, BIN1_Pin, BIN2_GPIO_Port, BIN2_Pin);
  Motor_Init(&motor[2], &htim4, &htim8, TIM_CHANNEL_3, CIN1_GPIO_Port, CIN1_Pin, CIN2_GPIO_Port, CIN2_Pin);
  Motor_Init(&motor[3], &htim5, &htim8, TIM_CHANNEL_4, DIN1_GPIO_Port, DIN1_Pin, DIN2_GPIO_Port, DIN2_Pin);

	motor[0].PidInit(&motor[0], PID_DELTA, 7200, 3600, 19, 0.9, 0);
  motor[1].PidInit(&motor[1], PID_DELTA, 7200, 3600, 19, 0.9, 0);
  motor[2].PidInit(&motor[2], PID_DELTA, 7200, 3600, 14, 0.9, 0);
  motor[3].PidInit(&motor[3], PID_DELTA, 7200, 3600, 19, 0.9, 0);

  while (1)
  {
    Vx = r->rocker[0].x_position;
    Vy = r->rocker[0].y_position;
    Va = r->rocker[1].x_position;
    Vz = r->rocker[1].y_position;//摇杆二控制旋转
		
		motor[0].speed_set = Vx + Vy - Vz;
    motor[1].speed_set = -Vx + Vy - Vz;

    motor[2].speed_set = -Vx - Vy - Vz;
    motor[3].speed_set = Vx - Vy - Vz;

    /*motor[0].speed_set = -Vx + Vy - Vz;
    motor[1].speed_set = -Vx - Vy + Vz;

    motor[2].speed_set = -Vx + Vy + Vz;
    motor[3].speed_set = -Vx - Vy - Vz;*/
    if(r->Switch[2])
      {
        motor[0].speed_set = Va;
        motor[1].speed_set = -Va;
        motor[2].speed_set = -Va;
        motor[3].speed_set = Va;
      }

    for (size_t i = 0; i < 4; i++)
    {
      motor[i].EncoderUpdate(&motor[i]);
    }
    for (size_t i = 0; i < 4; i++)
    {
      motor[i].SpeedGet(&motor[i]);
    }

    for (size_t i = 0; i < 4; i++)
    {
      motor[i].Calc(&motor[i]);
      if(motor[i].speed_set==0)//停止
      {
        motor[i].pid.out=0;
      }
      motor[i].Driver(&motor[i], motor[i].pid.out);
    }
    osDelayUntil(&time, 99);
  }
}