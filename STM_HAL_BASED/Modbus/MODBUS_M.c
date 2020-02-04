#include "MODBUS_M.h"
#include "stm32f4xx_hal.h"
#include "stdint.h"

extern huart2;









void process_read_registers()		//CRC CHECK FOR READ MULTIPLE REGISTERS FROM POINTED SLAVE
{
	int byte_cnt = rxData_Master[2]+2;
	uint16_t temp_registers[byte_cnt/2];	//STORE  DATA BYTES AND CRC BYTES TO TEMP_REG

	int j = 0;
	for(int i = 0; i<(byte_cnt/2);i++)
	{
		if(i == ((byte_cnt/2)-1))
		{
			temp_registers[i] = (rxData_Master[j+4] << 8) | rxData_Master[j+3];
		}
		else
		{
			temp_registers[i] = (rxData_Master[j+3] << 8) | rxData_Master[j+4];
			j += 2;
		}
	}
	uint8_t temp_rx[bytesToReceive];
	for(int i =0; i<(bytesToReceive); i++)
	{
		temp_rx[i] = rxData_Master[i];
	}
	uint16_t crc_ = CRC16(temp_rx, bytesToReceive-2);
	uint16_t temp_crc = temp_registers[(byte_cnt/2)-1];
	if (crc_ == temp_crc) 	//CHECKING RECEIVED CRC AND CALCULATED CRC
	{
		for(int i = 0; i<((byte_cnt/2)-1);i++)
		{
			id_one.Registers[reg_start_add+i] = temp_registers[i];
		}
		received_registers = 1;
	}
}

void POLL_Read_holding_registers(uint8_t slave_id,uint16_t starting_address,uint16_t qty )  //READS MULTIPLE REGISTERS FROM POINTED SLAVE
{
	uint8_t  holding_buffer[8];
	reg_start_add = starting_address;
	bytesToReceive = (qty * 2)+ 3 + 2;

	holding_buffer[0] = slave_id;
	holding_buffer[1] = 0x03;
	holding_buffer[2] = (uint8_t)(starting_address >> 8);
	holding_buffer[3] = (uint8_t)(starting_address);
	holding_buffer[4] = (uint8_t)(qty >> 8);
	holding_buffer[5] = (uint8_t)(qty);
	uint16_t crc_ = CRC16(holding_buffer,6);
	holding_buffer[6] = (crc_>>8);
	holding_buffer[7] = (uint8_t)crc_;
	if(HAL_UART_DMAStop(&huart2) == HAL_OK)
	{
		HAL_StatusTypeDef x = HAL_UART_Receive_DMA(&huart2, &rxData_Master[0],bytesToReceive);
		//HAL_Delay(1);	// use 10 if not working
		if(x == HAL_OK)
		{
			HAL_UART_Transmit(&huart2, holding_buffer, 8, 100);
		}
	}

}

void process_write_registers()	//CRC CHECK FOR WRITE SINGLE REGISTER TO POINTED SLAVE
{
	uint8_t byte_cnt = 2+2;
	uint16_t temp_registers[byte_cnt/2];
	int j = 0;
	for(int i = 0; i<(byte_cnt/2);i++)
	{
		if(i == ((byte_cnt/2)-1))
		{
			temp_registers[i] = (rxData_Master[j+5] << 8) | rxData_Master[j+4];
		}
		else
		{
			temp_registers[i] = (rxData_Master[j+4] << 8) | rxData_Master[j+5];
			j += 2;
		}
	}
	uint8_t temp_rx[bytesToReceive];
	for(int i =0; i<(bytesToReceive); i++)
	{
		temp_rx[i] = rxData_Master[i];
	}
	uint16_t crc_ = CRC16(temp_rx, bytesToReceive-2);
	uint16_t temp_crc = temp_registers[(byte_cnt/2)-1];
	if (crc_ == temp_crc) 	//CHECKING RECEIVED CRC AND CALCULATED CRC
	{
		sent_registers = 1;
	}
}

void POLL_Write_holding_registers(uint8_t slave_id,uint16_t address,uint16_t data)	//WRITES SINGLE REGISTER TO POINTED SLAVE
{
	uint8_t  holding_buffer[8];
	bytesToReceive = 8;
	holding_buffer[0] = slave_id;
	holding_buffer[1] = 0x06;
	holding_buffer[2] = (uint8_t)(address >> 8);
	holding_buffer[3] = (uint8_t)(address);
	holding_buffer[4] = (uint8_t)(data >> 8);
	holding_buffer[5] = (uint8_t)(data);
	uint16_t crc_ = CRC16(holding_buffer,6);
	holding_buffer[6] = (crc_>>8);
	holding_buffer[7] = (uint8_t)crc_;

	if(HAL_UART_DMAStop(&huart2) == HAL_OK)
	{
		HAL_StatusTypeDef x = HAL_UART_Receive_DMA(&huart2, &rxData_Master[0],bytesToReceive);
		//HAL_Delay(1);    // use 10 if not working
		if(x == HAL_OK)
		{
			HAL_UART_Transmit(&huart2, holding_buffer, 8, 100);
		}
	}
}

void POLL_Read_holding_coils(uint8_t slave_id,uint16_t starting_address,uint16_t qty)
{

}

unsigned int CRC16(const unsigned char *nData, unsigned int wLength)
{

	static const unsigned int wCRCTable[] = {
	    0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
	    0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
	    0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
	    0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
	    0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
	    0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
	    0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
	    0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
	    0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
	    0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
	    0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
	    0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
	    0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
	    0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
	    0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
	    0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
	    0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
	    0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
	    0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
	    0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
	    0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
	    0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
	    0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
	    0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
	    0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
	    0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
	    0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
	    0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
	    0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
	    0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
	    0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
	    0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
	  };


	  unsigned char nTemp;
	  unsigned int wCRCWord = 0xFFFF;

	  while (wLength--)
	  {
	    nTemp = *nData++ ^ wCRCWord;
	    wCRCWord >>= 8;
	    wCRCWord  ^= wCRCTable[nTemp];
	  }
	  return wCRCWord;
}

void clear_buffer_master()
{
	 for(int i = 0;i<sizeof(rxData_Master);i++)
	 {
		 rxData_Master[i] = 0;
	 }

}
