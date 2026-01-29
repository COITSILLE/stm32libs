#ifndef OLED_MENU_H
#define OLED_MENU_H

#include "oled.h"
#include "font.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Item_Typedef{
    const char* title;
    void (*command)(void);

    uint8_t rows;
    struct Item_Typedef* prev_item;
    struct Item_Typedef* next_item;
    struct Item_Typedef* top_sub_item;
    struct Item_Typedef* parent_item;
}Item_Typedef;

typedef struct Frame{
    uint8_t focus_item_y;
    pointer cur_item_ptr;
    Item_Typedef* cur_item;
    Item_Typedef* top_item;
    Item_Typedef* last_focus_item;
    uint8_t rows;
}Frame;

#define CONCAT(a,b) a##b
static Item_Typedef* init_last_item;
static Item_Typedef* init_parent_item;
static Item_Typedef* init_sub_item;
static Item_Typedef* menu_header;
static uint8_t menu_count;
static uint8_t add_submenu_sign;

#define BEGIN_MENU_SET\
    do{\
        init_last_item = NULL;\
        init_parent_item = NULL;\
        init_sub_item = NULL;\
        menu_header = NULL;\
        menu_count = 0;\
        add_submenu_sign = 0;\
    }while(0)\

#define _ADD_ITEM(_title_, _command_, count)\
    do{\
        if (add_submenu_sign == 1){\
            init_parent_item = init_last_item;\
            init_last_item = NULL;\
        }\
        static Item_Typedef CONCAT(item, count);\
        CONCAT(item, count).title        = _title_;\
        CONCAT(item, count).command      = _command_;\
        CONCAT(item, count).prev_item    = init_last_item;\
        CONCAT(item, count).next_item    = NULL;\
        CONCAT(item, count).top_sub_item = NULL;\
        CONCAT(item, count).parent_item  = init_parent_item;\
        if(menu_header == NULL && menu_count == 0)\
            menu_header = &CONCAT(item, count);\
        if(add_submenu_sign == 1){\
            init_parent_item->top_sub_item = &CONCAT(item, count);\
            add_submenu_sign = 0;\
        }\
        if (init_last_item != NULL)\
            init_last_item->next_item = &CONCAT(item, count);\
        init_last_item = &CONCAT(item, count);\
    }while(0)\

#define ADD_ITEM(title, command) _ADD_ITEM(title, command, __COUNTER__)

#define _ADD_SUBMENU_\
    do{\
        menu_count++;\
        add_submenu_sign = 1;\
        init_sub_item = init_last_item;\
    }while(0)\

#define _END_SUBMENU_\
    do{\
        menu_count--;\
        add_submenu_sign = 0;\
        init_last_item = init_parent_item;\
        if (init_parent_item != NULL)\
            init_parent_item = init_parent_item->parent_item;\
        else\
            init_parent_item = NULL;\
    }while(0)\

#define END_MENU_SET(header)\
    do{\
        init_last_item = NULL;\
        init_parent_item = NULL;\
        init_sub_item = NULL;\
        if (menu_count == 0)\
            header = menu_header;\
        else\
            header = NULL;\
        menu_header = NULL;\
        menu_count = 0;\
        add_submenu_sign = 0;\
    }while(0)\

#ifdef __cplusplus
}

template<class OLED_i>
class OledExMenu{
private:
    Item_Typedef* focus_item;
    pointer select_ptr;
    pointer item_ref_ptr;
    Frame frame;
public:
    Item_Typedef* header = NULL;
    OLED_i* oled;
    const Font* font;
    uint8_t rspacing;
    uint8_t cspacing;
    char pointer_char;

    OledExMenu(OLED_i *oled, const Font* font, uint8_t rspacing, uint8_t cspacing,
        char pointer_char = '>')
        : oled(oled), font(font), rspacing(rspacing), cspacing(cspacing),
        pointer_char(pointer_char) {}
    void init();
    void SetFrame();
    void GoNextItem();
    void GoPrevItem();
    void GoNextPage();
    void GoPrevPage();
    void Enter();
};

#ifndef OLED_EX_MENU_IPP
#include "oledex_menu.ipp"
#endif

#endif /* __cplusplus */

#endif