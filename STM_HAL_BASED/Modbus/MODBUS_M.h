/*
 * 24LC512.h
 *
 *  Created on: Feb 4, 2020
 *      Author: Deadshot
 */

#ifndef MODBUS_M_H
#define MODBUS_M_H



#include "stm32f4xx_hal.h"
#include "stdint.h"

typedef struct slaves
{
	uint16_t Registers[30];
	uint8_t Coils[10]; 
}SLAVE;

SLAVE id_one;
SLAVE id_two;

uint8_t received_registers,received_coils,sent_coils, sent_registers;

uint8_t rxData_Master[30],reg_start_add,coil_start_add;

uint8_t bytesToReceive;

void POLL_Read_holding_registers(uint8_t slave_id,uint16_t starting_address,uint16_t qty);

void POLL_Write_holding_registers(uint8_t slave_id,uint16_t address,uint16_t data );

void process_read_registers();

void process_write_registers();

void POLL_Read_holding_coils(uint8_t slave_id,uint16_t starting_address,uint16_t qty);

unsigned int CRC16(const unsigned char *nData, unsigned int wLength);

void clear_buffer_master();


#endif
