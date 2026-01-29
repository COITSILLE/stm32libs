#ifndef OLED_EX_MENU_IPP
#define OLED_EX_MENU_IPP

#ifdef __cplusplus

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "oledex_menu.h"

template <class OLED_i>
void OledExMenu<OLED_i>::init(){
    if (this->header == NULL){
        return;
    }
    this->focus_item = this->header;

    this->select_ptr = (pointer){0, 0};
    this->item_ref_ptr.x = this->font->height;
    this->item_ref_ptr.y = 0;

    this->frame.top_item = this->header;
    this->frame.rows = this->oled->GetPage() * 8 / this->cspacing;
}

#define ceil_div(a, b) (((a) + (b) - 1) / (b))

template <class OLED_i>
void OledExMenu<OLED_i>::SetFrame(){
    if (this->focus_item == NULL){
        return;
    }
    // Page change
    if (this->focus_item == this->frame.last_focus_item->top_sub_item ||
        this->focus_item == this->frame.last_focus_item->parent_item ||
        this->frame.last_focus_item == NULL
       )
    {
        this->frame.top_item = this->focus_item;
    }
    while (this->frame.top_item->prev_item != NULL){
        this->frame.top_item = this->frame.top_item->prev_item;
    }
    //Item change
    this->frame.cur_item = this->frame.top_item;
    int8_t roll_sign = 0;
    uint8_t rows = 0;
    if (this->focus_item != this->frame.top_item){
        while(this->frame.cur_item != NULL){
            rows += ceil_div(strlen(this->frame.cur_item->title) * this->rspacing, this->oled->GetCol());
            if (this->frame.cur_item == this->focus_item){
                roll_sign = 1;
                rows++;
                break;
            }
            this->frame.cur_item = this->frame.cur_item->next_item;
        }
    }
    if (roll_sign){
        if (rows > this->frame.rows){
            for (uint8_t k = 0; k < (rows - this->frame.rows); k++){
                if (this->frame.top_item->next_item != NULL)
                    this->frame.top_item = this->frame.top_item->next_item;
            }
        }
    }
    else{
        if (this->frame.top_item->prev_item != NULL)
            this->frame.top_item = this->frame.top_item->prev_item;
    }
    //draw frame
    this->oled->Clear();
    this->frame.cur_item = this->frame.top_item;
    this->frame.cur_item_ptr = this->item_ref_ptr;
    while(this->frame.cur_item_ptr.y < this->oled->GetPage() * 8){
        if (this->frame.cur_item == NULL){
            break;
        }
        if (this->frame.cur_item == this->focus_item){
            this->select_ptr.x = 0;
            this->select_ptr.y = this->frame.cur_item_ptr.y;
        }
        this->oled->SetString(this->frame.cur_item->title, &this->frame.cur_item_ptr, 
            this->font, this->rspacing, this->cspacing, 0);
        this->frame.cur_item = this->frame.cur_item->next_item;
        this->frame.cur_item_ptr.x = this->item_ref_ptr.x;
        this->frame.cur_item_ptr.y += this->cspacing;
    };
    this->oled->SetChar(this->pointer_char, &this->select_ptr, this->font);
    this->frame.last_focus_item = this->focus_item;
}

template <class OLED_i>
void OledExMenu<OLED_i>::GoNextItem(){
    if (this->focus_item->next_item != NULL)
        this->focus_item = this->focus_item->next_item;
    this->SetFrame();
    this->oled->ShowFrame();
}
template <class OLED_i>
void OledExMenu<OLED_i>::GoPrevItem(){
    if (this->focus_item->prev_item != NULL)
        this->focus_item = this->focus_item->prev_item;
    this->SetFrame();
    this->oled->ShowFrame();
}

template <class OLED_i>
void OledExMenu<OLED_i>::GoNextPage(){
    if(this->focus_item->top_sub_item != NULL)
        this->focus_item = this->focus_item->top_sub_item;
    this->SetFrame();
    this->oled->ShowFrame();
}

template <class OLED_i>
void OledExMenu<OLED_i>::GoPrevPage(){
    if(this->focus_item->parent_item != NULL)
        this->focus_item = this->focus_item->parent_item;
    this->SetFrame();
    this->oled->ShowFrame();
}

template <class OLED_i>
void OledExMenu<OLED_i>::Enter(){
    if (this->focus_item->command != NULL){
        this->focus_item->command();
    }
}


#endif

#endif