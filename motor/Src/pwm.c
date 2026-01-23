#include "pwm.h"
#include "stm32f1xx_hal_tim.h"
#include <stdint.h>

//TODO init that gets rid of CubeMx

void PWM_FreqInit(TIM_HandleTypeDef *PWMTIM, int pwm_freq){
    __HAL_TIM_SET_PRESCALER(PWMTIM, (APB1_FREQ / UNIT_FREQ) - 1);
    __HAL_TIM_SET_AUTORELOAD(PWMTIM, UNIT_FREQ / pwm_freq);
}

void PWM_InstanceInit(PWM_Typedef *pwm, TIM_HandleTypeDef *PWM_TIM, int Channel){
    pwm->Channel = Channel;
    pwm->PWM_TIM = PWM_TIM;
    
    HAL_TIM_PWM_Start(pwm->PWM_TIM, pwm->Channel);
    __HAL_TIM_SET_COMPARE(pwm->PWM_TIM, pwm->Channel, 0);
}

void PWM_SetDuty(PWM_Typedef *pwm, float duty_ratio){
    __HAL_TIM_SET_COMPARE(pwm->PWM_TIM, pwm->Channel, (duty_ratio / 100.0 * __HAL_TIM_GET_AUTORELOAD(pwm->PWM_TIM)));
}


