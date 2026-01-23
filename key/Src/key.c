#include "key.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

void Key_Init(Key_Typedef *key, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    key->GPIOx = GPIOx;
    key->GPIO_Pin = GPIO_Pin;
    key->state = KEY_RELEASED;
    key->debounce_time = 0;
}
void Key_Proc(Key_Typedef *key, KeyCallback callback){
    GPIO_PinState state = HAL_GPIO_ReadPin(key->GPIOx, key->GPIO_Pin);
    switch (state) {
        case GPIO_PIN_SET:
            switch (key->state) {
                case KEY_RELEASED:
                    key->state = KEY_DETECT;
                    key->debounce_time = HAL_GetTick();
                    break;
                case KEY_DETECT:
                    if (HAL_GetTick() - key->debounce_time > 20) {
                        key->state = KEY_PRESSED;
                    }
                    if (callback != NULL) callback();
                    break;
                case KEY_PRESSED:
                    key->state = KEY_WAITRELEASED;
                    break;
                case KEY_WAITRELEASED:
                    break;
                default:
                    break;
            }
            break;
        case GPIO_PIN_RESET:
            switch (key->state) {
                case KEY_RELEASED:
                        break;
                case KEY_DETECT:
                        key->state = KEY_RELEASED;
                     break;
                    case KEY_PRESSED:
                    key->state = KEY_RELEASED;
                        break;
                case KEY_WAITRELEASED:
                        key->state = KEY_RELEASED;
                        break;
                 default:
                       break;
            }
            break;
    }
}