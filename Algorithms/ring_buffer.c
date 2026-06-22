#include <stdio.h>
#include <stdint.h>

typedef struct{
const uint8_t *buffer;
int read_ptr;
int write_ptr;
int max_len;
}ring_st;

int write(ring_st *ring);
int read(ring_st *ring);

#define init_ring_st(x,y) uint8_t x##_data_buffer[y]; ring_st x = {.buffer = x##_data_buffer, .read_ptr=0, .write_ptr=0, .max_len=y}

init_ring_st(one,8);

int main()
{
	
	printf("%d",(int)sizeof(one_data_buffer));
	return 0;
}
