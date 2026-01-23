#include "sstrtotype.h"
#include <stdint.h>


#define NULL ((void*)0)

/**
 * @attention a correct str in the first call must be like "x,y,z\0", 
            while must NOT be like ",x,y,z\0", "x,,y,,,z\0" or "x,y,z,,\0" !
            Only single char delimiter is supported.
*/
char* s_strtok(char *str, const char delim){
    static char *saved_ptr = NULL;

    char *ptr;
    if (str == NULL){
        ptr = saved_ptr;
    }
    else{
        ptr = str;
        saved_ptr = NULL;
    }

    if (ptr == NULL) return NULL;

    uint8_t i = 0;
    while(*(ptr + i) != '\0'){
        if(*(ptr + i) == delim){
            *(ptr + i) = '\0';
            saved_ptr = ptr + i + 1;
            return ptr;
        }
        i++;
    }

    saved_ptr = NULL;
    return ptr;
}

/**
 * @attention a correct num must be like "1" while must NOT be like "a", "12", "-1" or "1a" !
*/
uint8_t s_atoi(const char num){
    if (num >= '0' && num <= '9') {
        return num - '0';
    }
    return 0;
}
float s_atof(const char *str){
    const char *ptr = str;
    float result = 0.0f;
    
    uint8_t result_sign = 0;
    uint8_t point_flag = 0;
    if (*ptr == '-'){
        result_sign = 1;
        ptr++;
    }

    float multiplier = 1.0f;
    while (*ptr != '\0'){
        if (*ptr == '.'){
            point_flag = 1;
            ptr++;
        }

        if (!point_flag){
            result = result * 10.0f + (float)s_atoi(*ptr);
        }
        else{
            multiplier *= 0.1f;
            result = result + (float)s_atoi(*ptr) * multiplier;
        }
        ptr++;
    }

    return result_sign ? -result : result;
}