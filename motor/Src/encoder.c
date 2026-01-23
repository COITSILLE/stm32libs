#include "encoder.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_cortex.h"
#include "stm32f1xx_hal_i2c.h"
#include "us_time.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_gpio.h"
#include <stdint.h>

#define IRQN_VOID 43

////DO change the 2 IRQ function's names according to the actual GPIO_Pin of PhraseA you are using!
#define USED_PIN1 GPIO_PIN_0
#define USED_PIN2 GPIO_PIN_1

typedef struct{
    uint64_t t0;
    uint64_t t1;
    uint64_t dt;
    int8_t dir;

    GPIO_TypeDef *PhraseA_GPIOx;
    uint16_t PhraseA_Pin;
    GPIO_TypeDef *PhraseB_GPIOx;
    uint16_t PhraseB_Pin;
}TimeParams;
static volatile TimeParams ta_l = {0, 1e10, 1, 0, 0, 0, 0};
static volatile TimeParams ta_r = {0, 1e10, 1, 0, 0, 0, 0};

void Encoder_InstanceInit(MotorEncoder_Typedef *encoder, GPIO_TypeDef *PhraseA_GPIOx, uint16_t PhraseA_Pin,
                        GPIO_TypeDef *PhraseB_GPIOx, uint16_t PhraseB_Pin, L_R_Index Index)
{
    encoder->PhraseA_GPIOx = PhraseA_GPIOx;
    encoder->PhraseA_Pin = PhraseA_Pin;
    encoder->PhraseB_GPIOx = PhraseB_GPIOx;
    encoder->PhraseB_Pin = PhraseB_Pin;
    encoder->Index = Index;
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (encoder->Index){
        ta_r.PhraseA_GPIOx = encoder->PhraseA_GPIOx;
        ta_r.PhraseA_Pin = encoder->PhraseA_Pin;
        ta_r.PhraseB_GPIOx = encoder->PhraseB_GPIOx;
        ta_r.PhraseB_Pin = encoder->PhraseB_Pin;
    }
    else{
        ta_l.PhraseA_GPIOx = encoder->PhraseA_GPIOx;
        ta_l.PhraseA_Pin = encoder->PhraseA_Pin;
        ta_l.PhraseB_GPIOx = encoder->PhraseB_GPIOx;
        ta_l.PhraseB_Pin = encoder->PhraseB_Pin;
    }

    if (encoder->PhraseA_GPIOx == GPIOA || encoder->PhraseB_GPIOx == GPIOA){
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    if (encoder->PhraseA_GPIOx == GPIOB || encoder->PhraseB_GPIOx == GPIOB){
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    if (encoder->PhraseA_GPIOx == GPIOC || encoder->PhraseB_GPIOx == GPIOC){
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
            
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(encoder->PhraseA_GPIOx, encoder->PhraseA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(encoder->PhraseB_GPIOx, encoder->PhraseB_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : Encoder_A_1_Pin */
    GPIO_InitStruct.Pin = encoder->PhraseA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(encoder->PhraseA_GPIOx, &GPIO_InitStruct);

    /*Configure GPIO pin : Encoder_B_1_Pin */
    GPIO_InitStruct.Pin = encoder->PhraseB_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(encoder->PhraseB_GPIOx, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    IRQn_Type IRQn_index = 0;
    switch (encoder->PhraseA_Pin) {
        case GPIO_PIN_0:
            IRQn_index = EXTI0_IRQn;
            break;
        case GPIO_PIN_1:
            IRQn_index = EXTI1_IRQn;
            break;
        case GPIO_PIN_2:
            IRQn_index = EXTI2_IRQn;
            break;
        case GPIO_PIN_3:
            IRQn_index = EXTI3_IRQn;
            break;
        case GPIO_PIN_4:
            IRQn_index = EXTI4_IRQn;
            break;
        default:
            IRQn_index = IRQN_VOID;
            break;
    }
    HAL_NVIC_SetPriority(IRQn_index, 2, 0);
    HAL_NVIC_EnableIRQ(IRQn_index);

}

//DO change the 2 IRQ function's names according to the actual GPIO_Pin of PhraseA you are using!

//leftmotor
void EXTI0_IRQHandler(){
    HAL_GPIO_EXTI_IRQHandler(USED_PIN1);
    ta_l.t0 = ta_l.t1;
    ta_l.t1 = ustime_GetUsTick();
    if (HAL_GPIO_ReadPin(ta_l.PhraseA_GPIOx, ta_l.PhraseA_Pin) 
        == HAL_GPIO_ReadPin(ta_l.PhraseB_GPIOx, ta_l.PhraseB_Pin)){
        if (ta_l.dir > 0){
            ta_l.dir = 0;
        }
        else {
            ta_l.dir = -1;
        }
        
    }
    else {
        if (ta_l.dir < 0){
            ta_l.dir = 0;
        }
        else {
            ta_l.dir = 1;
        }
    }
}

//rightmotor
void EXTI1_IRQHandler(){
    HAL_GPIO_EXTI_IRQHandler(USED_PIN2);
    ta_r.t0 = ta_r.t1;
    ta_r.t1 = ustime_GetUsTick();

    if (HAL_GPIO_ReadPin(ta_r.PhraseA_GPIOx, ta_r.PhraseA_Pin) 
        == HAL_GPIO_ReadPin(ta_r.PhraseB_GPIOx, ta_r.PhraseB_Pin)){
        if (ta_r.dir > 0){
            ta_r.dir = 0;
        }
        else {
            ta_r.dir = -1;
        }
    }
    else {
        if (ta_r.dir < 0){
            ta_r.dir = 0;
        }
        else {
            ta_r.dir = 1;
        }
    }
}

#define UNIT_RING 13
#define DECELARATE_RATIO 20
#define EDGE_PER_PERIOD 2

#define MAX(a, b) a > b ? a : b 

float Encoder_GetAngVelocity(MotorEncoder_Typedef *encoder){
    __disable_irq();
    int8_t dir_cpy = encoder->Index ? ta_r.dir : ta_l.dir;
    uint64_t t0_cpy = encoder->Index ? ta_r.t0 : ta_l.t0;
    uint64_t t1_cpy = encoder->Index ? ta_r.t1 : ta_l.t1;
    __enable_irq();

    float dt = MAX(t1_cpy - t0_cpy, ustime_GetUsTick() - t1_cpy) ;
    
    return dir_cpy / (dt * 1.0e-6f) * (360.0f / (UNIT_RING * EDGE_PER_PERIOD)) / DECELARATE_RATIO;
}
