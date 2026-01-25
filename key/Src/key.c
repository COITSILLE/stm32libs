#include "key.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

#define DEBOUNCE_DURATION 20U

void Key_Init(Key_Typedef *key, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, 
            GPIO_PinState valid_pin_state, KeyEdgeType edge_type, void (*KeyCallback)(void)){
    key->GPIOx = GPIOx;
    key->GPIO_Pin = GPIO_Pin;
    key->valid_pin_state = valid_pin_state;
    key->edge_type = edge_type;
    key->KeyCallback = KeyCallback;
    key->state = KEY_RELEASED;
    key->debounce_time = 0;
}
void Key_Proc(Key_Typedef *key){
    
    GPIO_PinState state = HAL_GPIO_ReadPin(key->GPIOx, key->GPIO_Pin);
    if (state == key->valid_pin_state) {
        switch (key->state) {
            case KEY_WAITRELEASED:
                break;
            case KEY_RELEASED:
                key->state = KEY_PDETECT;
                key->debounce_time = HAL_GetTick();
                break;
            case KEY_PDETECT:
                if (HAL_GetTick() - key->debounce_time > DEBOUNCE_DURATION) {
                    key->state = KEY_WAITRELEASED;
                    if (key->KeyCallback != NULL && (key->edge_type == RISING_EDGE || key->edge_type == BOTH_EDGE)) key->KeyCallback();
                }
                break;
            case KEY_RDETECT:
                key->state = KEY_WAITRELEASED;
                break;
            default:
                break;
            }
    }
    else{
        switch (key->state) {
            case KEY_RELEASED:
                break;
            case KEY_WAITRELEASED:
                key->state = KEY_RDETECT;
                key->debounce_time = HAL_GetTick();
                break;
            case KEY_RDETECT:
                if (HAL_GetTick() - key->debounce_time > DEBOUNCE_DURATION) {
                    key->state = KEY_RELEASED;
                    if (key->KeyCallback != NULL && (key->edge_type == FALLING_EDGE || key->edge_type == BOTH_EDGE)) key->KeyCallback();
                }
                break;
            case KEY_PDETECT:
                key->state = KEY_RELEASED;
                break;
            default:
                break;
            }
    }
}
