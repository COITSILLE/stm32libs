#ifndef MPU6050_H
#define MPU6050_H

#include "i2c.h"
#include <stdint.h>

void MPU6050_Reset();
void MPU6050_Init(uint8_t Accel_FS, uint16_t Gyro_FS, uint8_t DLPF_CFG, uint8_t SMPLRT_DIV, uint8_t SamplePeriod);
void MPU6050_Read_Accel(float* Accel_X, float* Accel_Y, float* Accel_Z);
void MPU6050_Read_Gyro(float* Gyro_X, float* Gyro_Y, float* Gyro_Z);
void MPU6050_GetOffset(uint8_t SampleTimes, float Expected_g);
void MPU6050_GetEurolianAngles(float* pitch, float* roll, float* yaw, 
                            float Accel_X, float Accel_Y, float Accel_Z, 
                            float Gyro_X, float Gyro_Y, float Gyro_Z,
                            float k);

#endif // MPU6050_H
