/*
 * mpu9250driver.c
 *
 *  Created on: 3 Oca 2023
 *      Author: alide
 */
#ifndef mpu9250
#define mpu9250
#include "mpu9250driver.h"
float ACCCONSTANT = 16384.0;
float GYROCONSTANT =131.0;
extern I2C_HandleTypeDef hi2c1;
struct RawData
{
    int16_t ax, ay, az, gx, gy, gz;
} extern rawData;

struct SensorData
{
    float ax, ay, az, gx, gy, gz;
} extern sensorData;

struct GyroCal
{
    float x, y, z;
} extern gyroCal;

uint8_t testDevice()
{
	uint8_t data;
	HAL_I2C_Mem_Read(&hi2c1, AD0_LOW, WHO_AM_I, 1, &data, 1, 10);

	if(data == 0x75) // CHANGE AD0_LOW here if you connect ado pin to high
		return 1;
	else
		return 0;
}
uint8_t beginmpu()
{
	uint8_t veri = testDevice();
	if(veri != 1)
		return 0;
	veri = 0;
	HAL_I2C_Mem_Write(&hi2c1, AD0_LOW, PWR_MGMT_1, 1,&veri, 1, 20);
	setRange();
	return 1;
}
void setRange()
{
	uint8_t data = 0x00;
 // we first set the accelerometer range , [4:3] bits in register varies the range. we will set register all 0 to use at 250.
	HAL_I2C_Mem_Write(&hi2c1, AD0_LOW, ACCEL_CONFIG, 1, &data , 1, 20);
	// now we set the  gyro range
	data = 0x00; // just to be sure
	HAL_I2C_Mem_Write(&hi2c1, AD0_LOW, GYRO_CONFIG, 1, &data , 1, 20);
}
void mpu_read()
{
	uint8_t BUFFER[14]; // WE START READ FROM  3B to 48 , every regiser has a byte data so we need 14 byte buffer
	HAL_I2C_Mem_Read(&hi2c1, AD0_LOW, ACCEL_XOUT_H, 1, BUFFER, 14, 50); // read the data
	rawData.ax = BUFFER[0] << 8 | BUFFER[1];
	rawData.ay = BUFFER[2] << 8 | BUFFER[3];
	rawData.az = BUFFER[4] << 8 | BUFFER[5];
	// NOW WE SHIFT AND SET THE GYROSCOPE DATA
	rawData.gx = BUFFER[8] << 8 | BUFFER[9];
	rawData.gy = BUFFER[10] << 8 | BUFFER[11];
	rawData.gz = BUFFER[12] << 8 | BUFFER[13];
}
void mpu_measure()
{
	mpu_read();
	// CONVER THE DATA WE READ BEFORE TO TYPE OF g(9.81m/s^2)
	sensorData.ax = rawData.ax/ACCCONSTANT;
	sensorData.ay = rawData.ay/ACCCONSTANT;
	sensorData.az = rawData.az/ACCCONSTANT;
	// CONVERT GYROSCOPE DATA TO degrees/second
	sensorData.gx = rawData.gx/GYROCONSTANT-gyroCal.x;
	sensorData.gy = rawData.gy/GYROCONSTANT-gyroCal.y;
	sensorData.gz = rawData.gz/GYROCONSTANT-gyroCal.z;
}
void calibrate_gyro()
{
	float x = 0;
	float y = 0;
	float z = 0;
	for(int i =0;i<500;i++)
	{
		mpu_measure();
		x += sensorData.gx;
		y += sensorData.gy;
		z += sensorData.gz;
		HAL_Delay(5);
	} // IT TAKES 2.5 SECONDS TO CALIBRATE
	gyroCal.x = x/500.0;
	gyroCal.y = y/500.0;
	gyroCal.z = z/500.0;
}
#endif
