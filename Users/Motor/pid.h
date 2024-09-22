#ifndef __PID_H__
#define __PID_H__

/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       pid.c/h
  * @brief      pid实现函数
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1.完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */

#include "main.h"
enum PID_MODE { PID_POSITION = 0, PID_DELTA };

typedef struct {
  uint8_t mode;
  // PID 三参数
  int16_t Kp;
  int16_t Ki;
  int16_t Kd;

  int16_t max_out;   //最大输出
  int16_t max_iout;  //最大积分输出

  int16_t set;
  int16_t fdb;

  int16_t out;
  int16_t Pout;
  int16_t Iout;
  int16_t Dout;
  int16_t Dbuf[3];   //微分项，0最新，1上一时刻，2上上时刻
  int16_t error[3];  //误差，0最新，1上一时刻，2上上时刻

} PidTypeDef;
extern void PID_Init(PidTypeDef *pid, uint8_t mode, float maxout, float max_iout, float kp, float ki, float kd);
extern int16_t PID_Calc(PidTypeDef *pid, int16_t ref, int16_t set);
extern void PID_clear(PidTypeDef *pid);

#endif  // !__PID_H__
