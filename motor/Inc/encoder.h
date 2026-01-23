#ifndef HAL_GPIO_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#endif

#ifndef ENCODER_H
#define ENCODER_H 

#include "main.h"
#include "stm32f103xb.h"
#include <stdint.h>

typedef enum {
    L = 0U,
    R = 1U
}L_R_Index;

typedef struct{
    GPIO_TypeDef *PhraseA_GPIOx;
    uint16_t PhraseA_Pin;

    GPIO_TypeDef *PhraseB_GPIOx;
    uint16_t PhraseB_Pin;

    L_R_Index Index;
}MotorEncoder_Typedef;

void Encoder_InstanceInit(MotorEncoder_Typedef *encoder, GPIO_TypeDef *PhraseA_GPIOx, uint16_t PhraseA_Pin,
                        GPIO_TypeDef *PhraseB_GPIOx, uint16_t PhraseB_Pin, L_R_Index Index);

//DO change the 2 IRQ function's names according to the actual GPIO_Pin of PhraseA you are using!

//leftmotor
void EXTI0_IRQHandler();
//rightmotor
void EXTI1_IRQHandler();

float Encoder_GetAngVelocity(MotorEncoder_Typedef *encoder);

#endif