#include "stm32f1xx_hal.h"
#define WHO_AM_I_6050_ANS 0x68
#define WHO_AM_I_9250_ANS 0x71
#define WHO_AM_I          117
#define AD0_LOW           (0x68<<1)
#define AD0_HIGH          0x69
#define GYRO_CONFIG       0x1B
#define ACCEL_CONFIG      0x1C
#define PWR_MGMT_1        0x6B
#define ACCEL_XOUT_H      0x3B
#define I2C_TIMOUT_MS     1000
#define RAD2DEG 57.2957795131
uint8_t testDevice();
uint8_t beginmpu();
uint8_t setConfig();
void setRange();
void mpu_read();
void mpu_measure();
void calibrate_gyro();

