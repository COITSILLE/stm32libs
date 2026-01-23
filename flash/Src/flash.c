#include "flash.h"
#include "stm32_hal_legacy.h"
#include "stm32f1xx_hal_flash_ex.h"
#include <stdint.h>
#include <sys/types.h>

#define FLASH_START_ADDR 0x0800FF00

void Flash_WriteFloat(uint8_t size, float *data, FLASHAddrs addr){

    HAL_FLASH_Unlock();
    __disable_irq();

    uint32_t error_addr;
    FLASH_EraseInitTypeDef FlashErase;
    FlashErase.TypeErase = FLASH_TYPEERASE_PAGES;
    FlashErase.PageAddress = addr;
    FlashErase.NbPages = 1;
    HAL_FLASHEx_Erase(&FlashErase, &error_addr);

    for (uint8_t i = 0; i < size; i++){
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + i * 4, *(uint32_t *)(data + i));
    }

    HAL_FLASH_Lock();
    __enable_irq();
}

void Flash_ReadFloat(uint8_t size, float *data, FLASHAddrs addr){
    for (uint8_t i = 0; i < size; i++){
        data[i] = *(float *)(addr + i * 4);
    }
}