#include <EEPROM.h>
#include <stm32f4xx.h>

extern I2C_HandleTypeDef hi2c1;

void write_onebyte(int address, char data)
{
	unsigned char buffer[3];
		buffer[0] = (unsigned char)(address>>8);
		buffer[1] = (unsigned char)(address & 0x00FF);
		buffer[3] = data;
		HAL_I2C_Master_Transmit(&hi2c1, EEPROM_ADD, buffer, sizeof(buffer), 100);
}

unsigned char read_onebyte(int address)
{
	unsigned char buffer[2],return_char;
	buffer[0] = (unsigned char)(address>>8);
	buffer[1] = (unsigned char)(address & 0x00FF);
	HAL_I2C_Master_Transmit(&hi2c1, EEPROM_ADD, buffer, sizeof(buffer), 100);
	HAL_I2C_Master_Receive(&hi2c1, EEPROM_ADD, return_char, 1, 100);
	return return_char;
}

float read_float(int address)
{
	float_buffer temp;
	int lock = address+4;
	for(int address,i=0; address<lock; address++,i++)
	{
		temp.dat[i] = read_onebyte(address);
	}
	return temp.f_data;
}

void write_float(int address,float data)
{
	float_buffer temp;
	temp.f_data = data;
	int lock = address+4;
	for(int address,i=0; address<lock; address++,i++)
	{
		 write_onebyte(address,temp.dat[i]);
	}
}
