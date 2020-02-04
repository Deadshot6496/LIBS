#include "MODBUS_SLAVE.h"
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart1;

void clear_buffer_slave()
{
	for(int x = 0; x<sizeof(buffer);x++)
	{
		buffer[x] = 0;
	}
}



int read_register(unsigned int start_add, unsigned int total)
{
	unsigned char byteCount = total * 2;
	unsigned char holding_reg[byteCount + 3 + 2];
	  holding_reg[0] = 0x01;
	  holding_reg[1] = 0x03;
	  holding_reg[2] = byteCount;
	  unsigned char index = 3;
	  for (int x = 0; x <= total; x++)
	  {
	    holding_reg[index + x] = Registers[start_add] >> 8;
	    index++;
	    holding_reg[index + x] = (unsigned char)Registers[start_add];
	    start_add++;
	  }
	  unsigned int crc = CRC16(holding_reg, (3 + byteCount));
	  holding_reg[byteCount + 3] = (unsigned char)(crc);
	  holding_reg[byteCount + 3 + 1] = (unsigned char)(crc >> 8);

	  for(int i = 0; i<sizeof(holding_reg);i++)
	  {
		  buffer[i] = holding_reg[i];
	  }
	  return sizeof(holding_reg);
}



int write_coils(unsigned int start_add, unsigned int data)
{
  unsigned char coil_byte = start_add / 8;
  unsigned char bit_shift = start_add % 8;//((start_add / 8) == 0) ? (8 - start_add) : ((start_add / 8) == 1) ? (16 - start_add) : ((start_add / 8) == 2) ? (24 - start_add) : 0;


  if (data == 0xFF00)
  {
    Coils[coil_byte] |= (1 << bit_shift);
  }
  else
  {
    Coils[coil_byte] &= ~(1 << bit_shift);
  }
  unsigned char holding_reg[8];
  holding_reg[0] = 0x01;
  holding_reg[1] = 0x05;
  holding_reg[2] = start_add >> 8;
  holding_reg[3] = (unsigned char)start_add;
  holding_reg[4] = data >> 8;
  holding_reg[5] = data;
  unsigned int crc = CRC16(holding_reg, 6);
  holding_reg[6] = (unsigned char)crc;
  holding_reg[7] = (unsigned char)(crc >> 8);

  for(int i = 0; i<sizeof(holding_reg);i++)
  {
	  buffer[i] = holding_reg[i];
  }

  return sizeof(holding_reg);
}

int write_reg(unsigned int start_add, unsigned int data)
{
  Registers[start_add] = data;
  unsigned char holding_reg[8];
  holding_reg[0] = 0x01;
  holding_reg[1] = 0x06;
  holding_reg[2] = (start_add >> 8);
  holding_reg[3] = (start_add);
  holding_reg[4] = data >> 8;
  holding_reg[5] = data;
  unsigned int crc = CRC16(holding_reg, 6);
  holding_reg[7] = crc >> 8;
  holding_reg[6] = crc;

  for(int i = 0; i<sizeof(holding_reg);i++)
  {
	  buffer[i] = holding_reg[i];
  }

  return sizeof(holding_reg);
}


int read_coils_(unsigned int startAdd, unsigned int total)    ///0x02
{
  unsigned char byteCount = ((total % 8) != 0) ? ((total / 8) + 1) : (total / 8);
  unsigned char temp_hold[byteCount];
  unsigned char coil_index = 0;
  unsigned char bit_count = 0, response_index = 0;
  unsigned int coil_bit_count = startAdd;
  unsigned char holding_reg[byteCount + 3 + 2];
  holding_reg[0] = 0x01;
  holding_reg[1] = 0x01;
  holding_reg[2] = byteCount;

  for (unsigned char x = 0; x <= total; x++)
  {
    if (bit_count < 8)
    {
      if (coil_bit_count < 8)
      {
        if (Coils[coil_index] & (1 << coil_bit_count))
        {
          temp_hold[response_index] |= (1 << bit_count);
        }
        else
        {
          temp_hold[response_index] &= ~(1 << bit_count);
        }
      }
      else
      {
        coil_bit_count = 0;
        coil_index++;
        if (Coils[coil_index] & (1 << coil_bit_count))
        {
          temp_hold[response_index] |= (1 << bit_count);
        }
        else
        {
          temp_hold[response_index] &= ~(1 << bit_count);
        }
      }
    }
    else
    {
      bit_count = 0;
      response_index++;
      if (coil_bit_count < 8)
      {
        if (Coils[coil_index] & (1 << coil_bit_count))
        {
          temp_hold[response_index] |= (1 << bit_count);
        }
        else
        {
          temp_hold[response_index] &= ~(1 << bit_count);
        }
      }
      else
      {
        coil_bit_count = 0;
        coil_index++;
        if (Coils[coil_index] & (1 << coil_bit_count))
        {
          temp_hold[response_index] |= (1 << bit_count);
        }
        else
        {
          temp_hold[response_index] &= ~(1 << bit_count);
        }
      }

    }
    bit_count++;
    coil_bit_count++;
  }
  for (int i = 3; i <= (byteCount + 3); i++)
  {
    holding_reg[i] = temp_hold[i - 3];
  }
  unsigned int crc = CRC16(holding_reg, byteCount + 3);
  holding_reg[byteCount + 3] = crc;
  holding_reg[byteCount + 1 + 3] = crc >> 8;

  for(int i = 0; i<sizeof(holding_reg);i++)
  {
	  buffer[i] = holding_reg[i];
  }

  return sizeof(holding_reg);
}

void coms()
{
	  if (rxData_slave[0] == 0x01)
	  {
	    switch (rxData_slave[1])
	    {
	      case 0x06:
	        {
	          uint16_t start_add = rxData_slave[2] << 8 | rxData_slave[3];
	          uint16_t data = rxData_slave[4] << 8 | rxData_slave[5];
	          int buffer_size = write_reg(start_add, data);
	          HAL_UART_Transmit_DMA(&huart1,buffer,buffer_size);
			  buffer_size = 0;
	        }
	        break;
	      case 0x03:
	        {
	          uint16_t start_add = rxData_slave[2] << 8 | rxData_slave[3];
	          uint16_t bytesToread = rxData_slave[4] << 8 | rxData_slave[5];
	          int buffer_size = read_register(start_add, bytesToread);
	          HAL_UART_Transmit_DMA(&huart1,buffer,buffer_size);
			  buffer_size = 0;
	        }
	        break;
	      case 0x01:
	        {
	          uint16_t start_add = rxData_slave[2] << 8 | rxData_slave[3];
	          uint16_t bytesToread = rxData_slave[4] << 8 | rxData_slave[5];
	          int buffer_size = read_coils_(start_add, bytesToread);
	          HAL_UART_Transmit_DMA(&huart1,buffer,buffer_size);
						buffer_size = 0;
	        } break;
	      case 0x05:
	        {
	          uint16_t start_add = rxData_slave[2] << 8 | rxData_slave[3];
	          uint16_t bytesToread = rxData_slave[4] << 8 | rxData_slave[5];
	          int buffer_size = write_coils(start_add, bytesToread);
	          HAL_UART_Transmit_DMA(&huart1,buffer,buffer_size);
						buffer_size = 0;
	        } break;
	      case 0x02:
	        {
	          uint16_t start_add = rxData_slave[2] << 8 | rxData_slave[3];
	          uint16_t bytesToread = rxData_slave[4] << 8 | rxData_slave[5];
	          int buffer_size = read_coils_(start_add, bytesToread);
	          HAL_UART_Transmit_DMA(&huart1,buffer,buffer_size);
						buffer_size = 0;
	        } break;
	      default :
	        {

	        }
	    }
	  }
}



