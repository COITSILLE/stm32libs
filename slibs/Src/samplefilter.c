#include "samplefilter.h"
#include "encoder.h"
#include "stm32f103xb.h"

#define FABS(a) a < 0 ? -a : a

float FirstOrderFilter_Get(FirstOrderFilter *filter, float value){
    filter->last_value = filter->alpha * value + (1.0 - filter->alpha) * filter->last_value;

    return filter->last_value;
}

float AdaptiveFirstOrderFilter_Get(AdaptiveFirstOrderFilter *filter, float value){
    
    if (FABS(value - filter->last_value) > filter->tolerance && filter->alpha < 1){
        filter->alpha_cpy = filter->alpha + 0.35;
    }
    else {
        filter->alpha_cpy = filter->alpha;
    }
    filter->last_value = filter->alpha_cpy * value + (1.0 - filter->alpha_cpy) * filter->last_value;
    return filter->last_value;

}

