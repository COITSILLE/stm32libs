#ifndef MOTOR_H
#define MOTOR_H

#include "encoder.h"
#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_tim.h"
#include "pwm.h"
#include "gpio.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

typedef enum{
    MOTOR_BRAKE = 0U,
    MOTOR_FORWARD = 1U,
    MOTOR_BACKWARD = 2U,
    MOTOR_SLIDE = 3U,
}MOTOR_STATE;

typedef struct{
    PWM_Typedef *MotorPWM;

    GPIO_TypeDef *Ctrl1GPIOx;
    uint16_t Ctrl1Pin;
    GPIO_TypeDef *Ctrl2GPIOx;
    uint16_t Ctrl2Pin;

    GPIO_TypeDef *StbyGPIOx;
    uint16_t StbyPin;
}Motor_Typedef;

float f_abs(float num);
void Motor_InstanceInit(Motor_Typedef *motor, PWM_Typedef *MotorPWM, GPIO_TypeDef *Ctrl1GPIOx,
                uint16_t Ctrl1Pin, GPIO_TypeDef *Ctrl2GPIOx, uint16_t Ctrl2Pin, GPIO_TypeDef *StbyGPIOx, 
                uint16_t StbyPin);
void Motor_SetDuty(Motor_Typedef *motor, float duty_ratio);
void Motor_SetControl(Motor_Typedef *motor, MOTOR_STATE State);
void Motor_STBY(Motor_Typedef *motor, GPIO_PinState STBY);

#endif