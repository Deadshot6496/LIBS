/*
 * MODBUS_SLAVE.h
 *
 *  Created on: Jan 27, 2020
 *      Author: DeadShot
 */

#ifndef MODBUS_SLAVE_H_
#define MODBUS_SLAVE_H_

#include "stdint.h"
#include "MODBUS_M.h"

uint8_t buffer[50],Coils[10],rxData_slave[8];

uint16_t Registers[300];



void clear_buffer_slave(void);

void coms(void);
int write_coils(unsigned int  start_add, unsigned int data);
int read_register(unsigned int start_add, unsigned int total);
int write_reg(unsigned int start_add, unsigned int data);
int read_coils_(unsigned int startAdd, unsigned int total);

#endif /* INC_MODBUS_SLAVE_H_ */
