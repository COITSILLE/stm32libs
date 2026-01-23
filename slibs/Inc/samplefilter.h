#ifndef FILTER_H
#define FILTER_H

#include "main.h"

typedef struct{
    float alpha;

    volatile float last_value;
}FirstOrderFilter;

typedef struct{
    float alpha;
    float tolerance;
    
    volatile float alpha_cpy;

    volatile float last_value;
}AdaptiveFirstOrderFilter;

float FirstOrderFilter_Get(FirstOrderFilter *filter, float value);
float AdaptiveFirstOrderFilter_Get(AdaptiveFirstOrderFilter *filter, float value);

#endif