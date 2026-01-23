#include "oled.h"
#include "main.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_gpio.h"
#include <stdint.h>
#include <string.h>
#include "font.h"
#include "math.h"
#include "stm32f1xx_hal_i2c.h"

#define I2C_Channel &hi2c1



uint8_t _DMA_Mode = 0;



//Basic functions
void OLED_SendCmd(uint8_t cmd) {
    uint8_t SendBuffer[2];
    SendBuffer[0] = 0x00; // Control byte for command
    SendBuffer[1] = cmd;
    if (_DMA_Mode){
        HAL_I2C_Master_Transmit_DMA(I2C_Channel, OLED_ADDRESS, SendBuffer, 2);
    }
    else{
        HAL_I2C_Master_Transmit(I2C_Channel, OLED_ADDRESS, SendBuffer, 2, HAL_MAX_DELAY);
    }
}



void OLED_Init(uint8_t DMA_Mode){
    // HAL_GPIO_WritePin(GPIOB, OLED_RES_Pin, 0);
    // HAL_Delay(200);
    // HAL_GPIO_WritePin(GPIOB, OLED_RES_Pin, 1);
    _DMA_Mode = DMA_Mode;
    HAL_Delay(50);

    OLED_SendCmd(0xAE);

    OLED_SendCmd(0xD5);
    OLED_SendCmd(0x80);

    OLED_SendCmd(0xA8);
    OLED_SendCmd(0x3F);

    OLED_SendCmd(0xD3);
    OLED_SendCmd(0x00);

    OLED_SendCmd(0x40);

    OLED_SendCmd(0xA1);

    OLED_SendCmd(0xC8);

    OLED_SendCmd(0xDA);
    OLED_SendCmd(0x12);

    OLED_SendCmd(0x81);
    OLED_SendCmd(0xCF);

    OLED_SendCmd(0xD9);
    OLED_SendCmd(0xF1);

    OLED_SendCmd(0xDB);
    OLED_SendCmd(0x30);

    OLED_SendCmd(0xA4);

    OLED_SendCmd(0xA6);

    OLED_SendCmd(0x8D);
    OLED_SendCmd(0x14);

    OLED_SendCmd(0xAF);

    HAL_Delay(100);
}

//Screen control
uint8_t GRAM[OLED_COL][OLED_ROW];

void OLED_Clear(){
    memset(GRAM, 0x00, sizeof(GRAM));
}

void OLED_ShowFrame(){
    uint8_t SendBuffer[OLED_ROW + 1];
    SendBuffer[0] = 0x40;
    for (uint8_t i = 0; i < OLED_COL; i++){
        for (uint8_t j = 0; j < OLED_ROW; j++){
            SendBuffer[j + 1] = GRAM[i][j];
        }
        OLED_SendCmd(0xB0 + i);
        OLED_SendCmd(0x00);
        OLED_SendCmd(0x10);

        HAL_I2C_Master_Transmit(I2C_Channel, OLED_ADDRESS, SendBuffer, sizeof(SendBuffer), HAL_MAX_DELAY);
    }
}

//basic edit GRAM functions
/**
 * @brief Set a pixel in the GRAM buffer
 * @param x Range 0-127(GRAM[][x]), begins at top left
 * @param y Range 0-63(GRAM[y/8][x], bit y%8) begins at top left
 * @param state 1 to set pixel, 0 to clear pixel
 */
void OLED_SetPixel(uint8_t x, uint8_t y, uint8_t state){
    if (x < 0 || x >= OLED_ROW || y < 0 || y >= OLED_COL * 8 || (state != 0 && state != 1)) {
        return;
    }
    GRAM[y / 8][x] |= (state << (y % 8));
}

/**
 * @brief Display a monochrome bitmap on the OLED screen
 * 
 * This function renders a bitmap image stored in program memory to the OLED display.
 * The bitmap data is stored in a byte array where each byte represents 8 horizontal pixels
 * (MSB left, LSB right), and the array is organized in row-major order.
 * 
 * @param picture Pointer to the bitmap data array
 * @param width Width of the bitmap in bytes (actual pixel width = width * 8)
 * @param height Height of the bitmap in pixels (number of rows)
 * @param ptrseq Index of the position pointer in the ptr array
 */
void OLED_SetPicture(uint8_t* picture, uint8_t width, uint8_t height, pointer* ptr){
    uint8_t width_r = width * 8;
    uint8_t scale = height * width;
    
    if (width_r + ptr->x > OLED_ROW || height + ptr->y > (OLED_COL * 8)) {
        return;
    }

    for (uint8_t i = 0; i < scale; i++) {
        uint8_t x_offset = (i % width) * 8;
        uint8_t y_offset = i / width;
        uint8_t byte = picture[i];
        
        for (uint8_t bit = 0; bit < 8; bit++) {
            uint8_t value = (byte >> bit & 0x01);
            OLED_SetPixel(ptr->x + x_offset + bit, ptr->y + y_offset, value);
        }
    }
}

//complex edit GRAM functions
void OLED_SetString(const char* string, pointer* ptr, const Font *font, uint8_t rspacing, uint8_t cspacing, uint8_t backpointer){
    uint8_t width_r = font->width * 8;
    uint8_t scale = font->height * font->width;

    int8_t rspc_offset = width_r - rspacing;
    int8_t cspc_offset = font->height - cspacing;

    uint8_t len = strlen(string);
    
    uint8_t x_0 = ptr->x;
    uint8_t y_0 = ptr->y;
    
    
    if ((len * (width_r - rspc_offset)) > (OLED_ROW * (OLED_COL * 8 / (font->height - cspc_offset)))){
        return;
    }

    for (uint8_t i = 0; i < len; i++){
        
        OLED_SetPicture(font->letter + ((uint8_t)string[i] - 32) * scale, 
                font->width, font->height, ptr);
        ptr->x += width_r - rspc_offset;
        if (ptr->x + width_r > OLED_ROW){
            ptr->x = x_0;
            ptr->y += font->height - cspc_offset;
        }
    }

    if (backpointer){
        ptr->x = x_0;
        ptr->y = y_0;
    }
}
//show


