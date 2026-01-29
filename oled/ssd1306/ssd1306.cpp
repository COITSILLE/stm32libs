#include "ssd1306.h"
#include <cstdint>
#include <cstring>
#include <vector>

void SSD1306_I2C::SendCmd(uint8_t cmd) {
    uint8_t SendBuffer[2];
    SendBuffer[0] = 0x00; // Control byte for command
    SendBuffer[1] = cmd;
    HAL_I2C_Master_Transmit(this->_hi2c_, this->_address_, SendBuffer, 2, HAL_MAX_DELAY);
}

void SSD1306_I2C::init(){
    memset(this->GRAM, 0, sizeof(this->GRAM));
    HAL_Delay(50);

    SendCmd(0xAE);

    SendCmd(0xD5);
    SendCmd(0x80);

    SendCmd(0xA8);
    SendCmd(0x3F);

    SendCmd(0xD3);
    SendCmd(0x00);

    SendCmd(0x40);

    SendCmd(0xA1);

    SendCmd(0xC8);

    SendCmd(0xDA);
    SendCmd(0x12);
    SendCmd(0x81);
    SendCmd(0xCF);

    SendCmd(0xD9);
    SendCmd(0xF1);

    SendCmd(0xDB);
    SendCmd(0x30);

    SendCmd(0xA4);

    SendCmd(0xA6);

    SendCmd(0x8D);
    SendCmd(0x14);

    SendCmd(0xAF);

    HAL_Delay(100);
}

void SSD1306_I2C::ShowFrame(){
    std::vector<uint8_t> SendBuffer(this->GetCol() + 1);
    SendBuffer[0] = 0x40;
    for (uint8_t i = 0; i < this->GetPage(); i++){
        for (uint8_t j = 0; j < this->GetCol(); j++){
            SendBuffer[j + 1] = this->GRAM[i][j];
        }
        SendCmd(0xB0 + i);
        SendCmd(0x00);
        SendCmd(0x10);

        HAL_I2C_Master_Transmit(this->_hi2c_, this->_address_, SendBuffer.data(), SendBuffer.size(), HAL_MAX_DELAY);
    }
}