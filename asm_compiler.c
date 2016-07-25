#include <stdio.h>
#include <stdlib.h>

uint16_t swap_16b( uint16_t data )
{

}

uint32_t swap_32b( uint32_t data )
{
	uint8_t part_0 = data;
	uint8_t part_1 = data >> 8;
	uint8_t part_2 = data >> 16;
	uint8_t part_3 = data >> 24;

	data = 0;

	data|= part_2;
	data = data << 8;

	data|= part_3;
	data = data << 8;

	data|= part_0;
	data = data << 8;

	data|= part_1;

	return data;
}

uint32_t emit_bl( uint32_t start, uint32_t offset )
{
	uint8_t symbol = 0;

	offset-= start;

	if( offset < 0 )
	{
		symbol = 1;
		offset*= -1;
	}

	uint16_t imm11 = offset >> 1;
	imm11&= 0b0000011111111111;

	uint16_t imm10 = offset >> 12;
	imm10&= 0b0000001111111111;

	uint8_t j = offset >> 21;
	j&= 0b00000011;

	uint32_t ready;

	ready = 0b111100 << 10;
	ready|= symbol;
	ready|= imm10;

	ready = ready << 5;
	ready|= 0b11010;

	ready|= j&0b01;

	ready|= (j&0b10) << 1;

	ready = ready << 11;
	ready|= imm11;

	return (swap_32b( ready ));
}
