#include "motor.h"
#include "pwm.h"
#include "stm32_hal_legacy.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_dma.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_tim.h"
#include "us_time.h"
#include <stdint.h>
#include <sys/_intsup.h>
#include <sys/types.h>


//TODO init that gets rid of CubeMx DONE

void Motor_InstanceInit(Motor_Typedef *motor, PWM_Typedef *MotorPWM, GPIO_TypeDef *Ctrl1GPIOx,uint16_t Ctrl1Pin, 
                        GPIO_TypeDef *Ctrl2GPIOx, uint16_t Ctrl2Pin, GPIO_TypeDef *StbyGPIOx, uint16_t StbyPin)
{
    motor->MotorPWM = MotorPWM;
    motor->Ctrl1GPIOx = Ctrl1GPIOx;
    motor->Ctrl1Pin = Ctrl1Pin;
    motor->Ctrl2GPIOx = Ctrl2GPIOx;
    motor->Ctrl2Pin = Ctrl2Pin;
    motor->StbyGPIOx = StbyGPIOx;
    motor->StbyPin = StbyPin;

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (motor->Ctrl1GPIOx == GPIOA || motor->Ctrl2GPIOx == GPIOA || motor->StbyGPIOx == GPIOA){
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
    }
    if (motor->Ctrl1GPIOx == GPIOB || motor->Ctrl2GPIOx == GPIOB || motor->StbyGPIOx == GPIOB){
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    if (motor->Ctrl1GPIOx == GPIOC || motor->Ctrl2GPIOx == GPIOC || motor->StbyGPIOx == GPIOC){
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }

    HAL_GPIO_WritePin(motor->Ctrl1GPIOx,motor->Ctrl1Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(motor->Ctrl2GPIOx, motor->Ctrl2Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(motor->StbyGPIOx, motor->StbyPin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = motor->Ctrl1Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(motor->Ctrl1GPIOx, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = motor->Ctrl2Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(motor->Ctrl2GPIOx, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = motor->StbyPin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(motor->StbyGPIOx, &GPIO_InitStruct);
}


void Motor_STBY(Motor_Typedef *motor, GPIO_PinState STBY){
    HAL_GPIO_WritePin(motor->StbyGPIOx, motor->StbyPin, STBY);
}

void Motor_SetDuty(Motor_Typedef *motor, float duty_ratio){
    float duty;
    if (duty_ratio < 0){
        duty = 0;
    }
    else if (duty_ratio > 100){
        duty = 100;
    }
    else{
        duty = duty_ratio;
    }

    PWM_SetDuty(motor->MotorPWM, duty);
}

void Motor_SetControl(Motor_Typedef *motor, MOTOR_STATE State){
    switch (State) {
        case MOTOR_BRAKE:
            HAL_GPIO_WritePin(motor->Ctrl1GPIOx, motor->Ctrl1Pin, 1);
            HAL_GPIO_WritePin(motor->Ctrl2GPIOx, motor->Ctrl2Pin, 1);
            break;
        case MOTOR_FORWARD:
            HAL_GPIO_WritePin(motor->Ctrl1GPIOx, motor->Ctrl1Pin, 0);
            HAL_GPIO_WritePin(motor->Ctrl2GPIOx, motor->Ctrl2Pin, 1);
            break;
        case MOTOR_BACKWARD:
            HAL_GPIO_WritePin(motor->Ctrl1GPIOx, motor->Ctrl1Pin, 1);
            HAL_GPIO_WritePin(motor->Ctrl2GPIOx, motor->Ctrl2Pin, 0);
            break;
        case MOTOR_SLIDE:
            HAL_GPIO_WritePin(motor->Ctrl1GPIOx, motor->Ctrl1Pin, 0);
            HAL_GPIO_WritePin(motor->Ctrl2GPIOx, motor->Ctrl2Pin, 0);
            break;
    }
}






