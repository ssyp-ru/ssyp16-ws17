#include <stdio.h>
#include <stdlib.h>

uint16_t swap_16b( uint16_t data )
{

	uint8_t part_0 = data;

	data = data >> 8;

	data|= ((uint16_t)part_0) << 8;

	return data;
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
	int S = 0;

	offset-= start;

	if( offset < 0 )
	{
		S = 1;
		offset*= -1;
	}

	uint16_t imm11 = offset >> 1;
	imm11&= 0b0000011111111111;

	uint16_t imm10 = offset >> 12;
	imm10&= 0b0000001111111111;

	uint8_t j = offset >> 21;
	//j&= 0b00000011;
	if( !S )
	{
		j = ~j;
	}

	j&= 0b00000011;

	uint32_t ready;

	ready = 0b111100 << 10;
	ready|= S;
	ready|= imm10;

	ready = ready << 5;
	ready|= 0b11010;

	ready|= j&0b01;

	ready|= (j&0b10) << 1;

	ready = ready << 11;
	ready|= imm11;

	return (swap_32b( ready ));
}

//0-7 = R0-R7 registr, 8 = LR
uint16_t emit_push( int registr )
{
	uint16_t ready = 0b1011010000000000;
	ready|= 1 << registr;

	return (swap_16b( ready ));
}

uint16_t emit_pop( int registr )
{
	uint16_t ready = 0b1011110000000000;
	ready|= 1 << registr;

	return (swap_16b( ready ));
}

//offset = {0,1020}
uint16_t emit_ldr_short( int offset, int registr )
{
	uint16_t ready = 0b0100100000000000;
	registr&= 0b111;

	reeady|= (uint8_t)(offset >> 2);

	return (swap_16b( ready ));
}

uint32_t emit_ldr_long( int offset, int registr )
{
	uint32_t ready = 0b1111100011011111;

	if( offset < 0 )
	{
		ready&= ~((uint32_t)( 1 << 7 ));
		offset*= -1;
	}

	ready = ready << 3;
	ready|= ( registr & 0b1111 );

	ready = ready << 12;
	ready|= ( offset & 0b111111111111 );

	return (swap_32b( ready ));
}
