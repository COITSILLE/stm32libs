#include "us_time.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h"
#include <stdint.h>

#define USTIME_TIM htim4

//TODO init that gets rid of CubeMx DONE

TIM_HandleTypeDef USTIME_TIM;

void ustime_Init(){
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim4.Instance = USTIME_TIM.Instance;
    htim4.Init.Prescaler = 72-1;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = 1000-1;
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    //if (HAL_TIM_Base_Init(&USTIME_TIM) != HAL_OK)
    // {
    //     Error_Handler();
    // }
    /* Check the TIM handle allocation */
    USTIME_TIM.Lock = HAL_UNLOCKED;
    __HAL_RCC_TIM4_CLK_DISABLE();
    USTIME_TIM.State = HAL_TIM_STATE_BUSY;

    /* Set the Time Base configuration */
    TIM_Base_SetConfig(USTIME_TIM.Instance, &USTIME_TIM.Init);

    /* Initialize the DMA burst operation state */
    USTIME_TIM.DMABurstState = HAL_DMA_BURST_STATE_READY;

    /* Initialize the TIM channels state */
    TIM_CHANNEL_STATE_SET_ALL(&USTIME_TIM, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_N_STATE_SET_ALL(&USTIME_TIM, HAL_TIM_CHANNEL_STATE_READY);

    /* Initialize the TIM state*/
    USTIME_TIM.State = HAL_TIM_STATE_READY;

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&USTIME_TIM, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&USTIME_TIM, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    uint32_t lastmm = HAL_GetTick();
    while(HAL_GetTick() - lastmm == 0);
    HAL_TIM_Base_Start(&USTIME_TIM);
}
uint64_t ustime_GetUsTick(){
    return HAL_GetTick() * 1000 + __HAL_TIM_GET_COUNTER(&USTIME_TIM);
    
}
//TODO ustime_Delay()

