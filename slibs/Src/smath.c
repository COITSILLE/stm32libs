#include "smath.h"

float s_fabs(float x){
    return x < 0 ? -x : x;
}

float s_max(float a, float b){
    return a > b ? a : b;
}

float s_min(float a, float b){
    return a < b ? a : b;
}

float s_powf(float base, uint8_t exp){
    float result = 1.0;
    for (int i = 0; i < exp; i++){
        result *= base;
    }
    return result;
}
