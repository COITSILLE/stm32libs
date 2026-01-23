#ifndef INC_OLED_H
#define INC_OLED_H

#include "i2c.h"
#include "font.h"

#define OLED_ADDRESS 0x78
//#define OLED_ADDRESS 0x3C

//#define OLED_ADDRESS 0x38

void OLED_SendCmd(uint8_t cmd);
void OLED_Init(uint8_t DMA_Mode);

void OLED_Clear();
void OLED_SetPointer(uint8_t x, uint8_t y, uint8_t seq);
void OLED_ShowFrame();
void OLED_SetPixel(uint8_t x, uint8_t y, uint8_t state);
void OLED_SetPicture(uint8_t* picture, uint8_t width, uint8_t height, uint8_t ptrseq);
void OLED_SetString(const char* string, uint8_t ptrseq, const Font *font, uint8_t rspacing, uint8_t cspacing, uint8_t backpointer);

#endif /* INC_OLED_H_ */