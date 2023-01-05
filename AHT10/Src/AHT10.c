#include "AHT10.h"
#include "stm32f1xx_hal.h"
#define DEVICE_ADDR 0x38 << 1
struct{
	float nem;
	float sicaklik;
} extern veri;
uint8_t BUFFER[6];
uint8_t measure = 0xAC;
extern I2C_HandleTypeDef hi2c1;
void AHT10enableSensor()
{
	uint8_t data = 0xE1;
	HAL_I2C_Master_Transmit(&hi2c1, DEVICE_ADDR, &data , 1,10);
}
void AHT10measure() // BEFORE READ THE MEASURED DATA YOU MOST WAIT AT LEAST 75MS !!!
{
	HAL_I2C_Master_Transmit(&hi2c1, DEVICE_ADDR, &measure , 1,10);

}
void AHT10read()
{
	HAL_I2C_Master_Receive(&hi2c1, DEVICE_ADDR, BUFFER, 6, 50);
	uint32_t traw = (((uint32_t)BUFFER[3] & 15) << 16) | ((uint32_t)BUFFER[4] << 8) | BUFFER[5];
	uint32_t hraw =((BUFFER[1] << 16) | (BUFFER[2] << 8) | BUFFER[3]) >> 4;
	veri.sicaklik  = (float)(traw * 200.00 / 1048576.00) - 50.00;
	veri.nem = (hraw * 100 / 1048576);
}
