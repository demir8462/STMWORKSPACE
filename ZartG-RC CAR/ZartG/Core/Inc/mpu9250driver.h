#include "stm32f1xx_hal.h"
#define RAD2DEG 57.295779513082320876798154814105

#define WHO_AM_I 0x75
#define PWR_MGMT_1 0x6B
#define SMPLRT_DIV 0x19
#define ACCEL_CONFIG 0x1C
#define ACCEL_XOUT_H 0x3B
#define TEMP_OUT_H 0x41
#define GYRO_CONFIG 0x1B
#define GYRO_XOUT_H 0x43

// Setup MPU6050
#define AD0_LOW (0x68 << 1)
uint8_t testDevice();
uint8_t beginmpu();
uint8_t setConfig();
void setRange();
void mpu_read();
void mpu_measure();
void calibrate_gyro();

