#ifndef FLASH_H
#define FLASH_H 

#include "main.h"
#include <stdint.h>

typedef enum{
    PAGE60 = 0x0800F000U,
    PAGE61 = 0x0800F400U,
    PAGE62 = 0x0800F800U,
    PAGE63 = 0x0800FC00U,
}FLASHAddrs;

typedef struct{
    float *val;
    FLASHAddrs addr;
}PerminaentFloat_Typedef;

typedef struct{
    uint32_t *val;
    uint32_t addr;
}PerminaentInteger_Typedef;

void Flash_WriteFloat(uint8_t size,float *data, FLASHAddrs addr);
void Flash_ReadFloat(uint8_t size, float *data, FLASHAddrs addr);

#endif