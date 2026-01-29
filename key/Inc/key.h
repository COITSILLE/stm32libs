#ifndef KEY_H
#define KEY_H

#include "main.h"
#include "stm32f103xb.h"

typedef enum{
    KEY_PDETECT = 2U,
    KEY_RDETECT = 3U,
    KEY_WAITRELEASED = 4U,
    KEY_RELEASED = 5U
} KeyState;

typedef enum{
    RISING_EDGE,
    FALLING_EDGE,
    BOTH_EDGE
} KeyEdgeType;

typedef struct{
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
    GPIO_PinState valid_pin_state;
    KeyEdgeType edge_type;
    void (*KeyCallback)(void);

    KeyState state;
    uint32_t debounce_time;
} Key_Typedef;

void Key_Init(Key_Typedef *key, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, 
            GPIO_PinState valid_pin_state, KeyEdgeType edge_type, void (*KeyCallback)(void));
void Key_Proc(Key_Typedef *key);
#endif
