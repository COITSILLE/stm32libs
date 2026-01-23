#ifndef PWM_H
#define PWM_H 

#include "main.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h"
#include <sys/types.h>

#define APB1_FREQ 72000000
#define UNIT_FREQ 1000000

typedef struct{
    TIM_HandleTypeDef *PWM_TIM;
    int Channel;
}PWM_Typedef;

void PWM_FreqInit(TIM_HandleTypeDef *PWMTIM, int pwm_freq);
void PWM_InstanceInit(PWM_Typedef *pwm, TIM_HandleTypeDef *PWM_TIM, int Channel);
void PWM_SetDuty(PWM_Typedef *pwm, float duty_ratio);

#endif