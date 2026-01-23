#ifndef HAL_TIM_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#endif

#ifndef US_TIME_H
#define US_TIME_H

#include "main.h"
#include <stdint.h>

void ustime_Init();
uint64_t ustime_GetUsTick();

#endif