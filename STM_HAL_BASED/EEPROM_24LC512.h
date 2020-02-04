/*
 * 24LC512.h
 *
 *  Created on: Feb 4, 2020
 *      Author: Deadshot
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#define EEPROM_ADD (uint8_t)0x50

typedef union{
	unsigned char dat[4];
	float f_data;
}float_buffer;

void write_onebyte(int address, char data);

unsigned char read_onebyte(int address);

void write_float(int address,float data);

float read_float(int address);

#endif /* INC_EEPROM_H_ */
