#ifndef PID_H
#define PID_H 

#include "main.h"
#include "flash.h"

typedef struct{
    float K_p;
    float K_i;
    float K_d;

    float MaxOutput;
    float MinOutput;
    float KiLimit;

    float SP;

    //volatile
    volatile uint64_t last_time;
    volatile float last_err;
    volatile float err_i;
}PID_Typedef;

void PID_ReInit(PID_Typedef *pid);
void PID_Init(PID_Typedef *pid, FLASHAddrs addr, uint8_t do_read_limits);
void PID_SetSP(PID_Typedef *pid, float sp);
void PID_SetFactors(PID_Typedef *pid, float kp, float ki, float kd);
void PID_SetLimits(PID_Typedef *pid, float max_output, float min_output, float ki_limit);
float PID_GetCO(PID_Typedef *pid, float FB);
void PID_SaveParams(PID_Typedef *pid, FLASHAddrs addr);
void PID_ReadParams(PID_Typedef *pid, FLASHAddrs addr);
#endif