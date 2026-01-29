#ifndef INC_OLED_H
#define INC_OLED_H

#include "font.h"
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef struct{
    uint8_t x;
    uint8_t y;
}pointer;

#ifdef __cplusplus
}
template<size_t COL, size_t PAGE>
class OLED_Base{
protected:
    uint8_t GRAM[PAGE][COL] = {};
    virtual void SendCmd(uint8_t cmd) = 0;
    
public:
    OLED_Base() = default;

    virtual void init() = 0;
    void Clear();
    virtual void ShowFrame() = 0;
    void SetPixel(uint8_t x, uint8_t y, uint8_t state);
    void SetPicture(uint8_t* picture, uint8_t width, uint8_t height, pointer* ptr);
    void SetChar(char character, pointer* ptr, const Font *font);
    void SetString(const char* string, pointer* ptr, const Font *font,
        uint8_t rspacing, uint8_t cspacing, uint8_t backpointer = 1);
    
    constexpr static uint16_t GetCol(){ return COL;};
    constexpr static uint8_t GetPage(){ return PAGE;};
};

#ifndef OLED_IPP
#include "oled.ipp"
#endif

#endif /* __cplusplus */

#endif /* INC_OLED_H_ */