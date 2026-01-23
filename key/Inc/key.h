#ifndef KEY_H
#define KEY_H

#include "main.h"
#include "stm32f103xb.h"

typedef enum{
    KEY_DETECT = 2U,
    KEY_PRESSED = 3U,
    KEY_WAITRELEASED = 4U,
    KEY_RELEASED = 5U
} KeyState;

typedef struct{
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
    KeyState state;
    uint32_t debounce_time;
} Key_Typedef;

typedef void (*KeyCallback)(void);

void Key_Init(Key_Typedef *key, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Key_Proc(Key_Typedef *key, KeyCallback callback);
#endif