#include "mpu6050.h"
#include "i2c.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_i2c.h"
#include "stm32f1xx_hal_uart.h"
#include <stdint.h>
#include <sys/types.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define I2C_Channel &hi2c1

#define SMPRT_DIV 0x19

#define MPU6050_address 0x68 << 1
static struct{
    float AccelFactor;
    float GyroFactor;

    float g;

    uint8_t SamplePeriod;
}cfg = {0.0, 0.0, 9.8f, 0};

static struct{
    float Accel_X, Accel_Y, Accel_Z;
    float Gyro_X, Gyro_Y, Gyro_Z;
}offsets = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};




void MPU6050_Reset(){
    uint8_t cmd[2] = {0x6B, 0b10000000};
    HAL_I2C_Master_Transmit(I2C_Channel, MPU6050_address, cmd, sizeof(cmd), HAL_MAX_DELAY);
}

void SetReg(uint8_t reg, uint8_t value){
    uint8_t cmd[2] = {reg, value};
    HAL_I2C_Master_Transmit(I2C_Channel, MPU6050_address, cmd, sizeof(cmd), HAL_MAX_DELAY);
}

/**
 * @brief  Initialize the MPU6050
 * @param  Accel_FS: Accelerometer Full Scale. 2/4/8/16g
 * @param  Gyro_FS: Gyroscope Full Scale. 250/500/1000/2000dps
 * @param  DLPF_CFG: Digital Low Pass Filter Configuration. 0-7. Recommended value: 3
 * @param  SMPLRT_DIV: Sample Rate Division. 0-7. Recommended value: 4, when DLPF_CFG = 3
*/
void MPU6050_Init(uint8_t Accel_FS, uint16_t Gyro_FS, uint8_t DLPF_CFG, uint8_t SMPLRT_DIV, uint8_t SamplePeriod){
    SetReg(0x6B, 0b00000000);// Wake up device

    cfg.AccelFactor = (1.0f / cfg.g) * 32768.0f / Accel_FS;
    cfg.GyroFactor = 32768.0f / Gyro_FS;
    cfg.SamplePeriod = SamplePeriod;

    switch (Accel_FS) {
        case 2:
            SetReg(0x1C, 0b00000000);// Accel +/- 2g
            break;
        case 4:
            SetReg(0x1C, 0b00001000);// Accel +/- 4g
            break;
        case 8:
            SetReg(0x1C, 0b00010000);// Accel +/- 8g
            break;
        case 16:
            SetReg(0x1C, 0b00011000);// Accel +/- 16g
            break;
        default:
            SetReg(0x1C, 0b00000000);// Accel +/- 2g
            break;
    }

    switch (Gyro_FS) {
        case 250:
            SetReg(0x1B, 0b00000000);// Gyro +/- 250 deg/s
            break;
        case 500:
            SetReg(0x1B, 0b00001000);// Gyro +/- 500 deg/s
            break;
        case 1000:
            SetReg(0x1B, 0b00010000);// Gyro +/- 1000 deg/s
            break;
        case 2000:
            SetReg(0x1B, 0b00011000);// Gyro +/- 2000 deg/s
            break;
        default:
            SetReg(0x1B, 0b00000000);// Gyro +/- 250 deg/s
            break;
    
    }
    SetReg(0x1A, DLPF_CFG & 0b00000111);// DLPF 
    SetReg(SMPRT_DIV, SMPLRT_DIV & 0b00000111);// Sample Rate division

}



void MPU6050_Read_Accel(float* Accel_X, float* Accel_Y, float* Accel_Z){
    uint8_t reg = 0x3B;
    uint8_t rawdata[6];

    HAL_I2C_Master_Transmit(I2C_Channel, MPU6050_address, &reg, 1, HAL_MAX_DELAY);

    HAL_I2C_Master_Receive(I2C_Channel, MPU6050_address, rawdata, sizeof(rawdata), HAL_MAX_DELAY);

    *Accel_X = (float)((int16_t)(rawdata[0] << 8 | rawdata[1]) / cfg.AccelFactor - offsets.Accel_X);
    *Accel_Y = (float)((int16_t)(rawdata[2] << 8 | rawdata[3]) / cfg.AccelFactor - offsets.Accel_Y);
    *Accel_Z = (float)((int16_t)(rawdata[4] << 8 | rawdata[5]) / cfg.AccelFactor - offsets.Accel_Z);
}

void MPU6050_Read_Gyro(float* Gyro_X, float* Gyro_Y, float* Gyro_Z){
    uint8_t reg = 0x43;
    uint8_t rawdata[6];

    HAL_I2C_Master_Transmit(I2C_Channel, MPU6050_address, &reg, 1, HAL_MAX_DELAY);

    HAL_I2C_Master_Receive(I2C_Channel, MPU6050_address, rawdata, sizeof(rawdata), HAL_MAX_DELAY);

    *Gyro_X = (float)((int16_t)(rawdata[0] << 8 | rawdata[1]) / cfg.GyroFactor - offsets.Gyro_X);
    *Gyro_Y = (float)((int16_t)(rawdata[2] << 8 | rawdata[3]) / cfg.GyroFactor - offsets.Gyro_Y);
    *Gyro_Z = (float)((int16_t)(rawdata[4] << 8 | rawdata[5]) / cfg.GyroFactor - offsets.Gyro_Z);
}

/**
 * @brief  Get the offset of the sensor
 * @param  SampleTimes: Number of samples to take
 * @param  Expected_g: Expected g value
 * @attention DO NOT run this function while the sensor is moving
*/
//TODO:Offset store permenantly in FALSH
void MPU6050_GetOffset(uint8_t SampleTimes, float Expected_g){ 
    cfg.g = Expected_g;
    float Accel[3];
    float Gyro[3];
    float Accel_Sum[3] = {0,0,0};
    float Gyro_Sum[3] = {0,0,0};
    for (uint8_t i = 0; i < SampleTimes; i++){
        MPU6050_Read_Accel(&Accel[0], &Accel[1], &Accel[2]);
        MPU6050_Read_Gyro(&Gyro[0], &Gyro[1], &Gyro[2]);
        for (uint8_t j = 0; j < 3; j++){
            Accel_Sum[j] += Accel[j];
            Gyro_Sum[j] += Gyro[j];
        }
        HAL_Delay(cfg.SamplePeriod);
    }

    offsets.Accel_X = (float)Accel_Sum[0] / SampleTimes;
    offsets.Accel_Y = (float)Accel_Sum[1] / SampleTimes;
    offsets.Accel_Z = (float)Accel_Sum[2] / SampleTimes - Expected_g;
    offsets.Gyro_X = (float)Gyro_Sum[0] / SampleTimes;
    offsets.Gyro_Y = (float)Gyro_Sum[1] / SampleTimes;
    offsets.Gyro_Z = (float)Gyro_Sum[2] / SampleTimes;


}

void MPU6050_GetEurolianAngles(float* pitch, float* roll, float* yaw, 
                            float Accel_X, float Accel_Y, float Accel_Z, 
                            float Gyro_X, float Gyro_Y, float Gyro_Z,
                            float k){
    float dt = cfg.SamplePeriod / 1000.0f;
    //From Accel
    float Inv_Pi = 180.0 / M_PI;
    float pitch_accel = atan2(Accel_Y, Accel_Z) * Inv_Pi;
    float roll_accel = -atan2(Accel_X, sqrt(Accel_Z * Accel_Z + Accel_Y * Accel_Y)) * Inv_Pi;
    //From Gyro
    float pitch_gyro = *pitch + Gyro_X * dt;
    float roll_gyro = *roll + Gyro_Y * dt;
    float yaw_gyro = *yaw + Gyro_Z * dt;

    *pitch = k * pitch_gyro + (1 - k) * pitch_accel;
    *roll = k * roll_gyro + (1 - k) * roll_accel;
    *yaw = yaw_gyro;

}