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
	int S = 1;

	offset-= start;

	if( offset < 0 )
	{
		S = 0;
		offset*= -1;
	}

	uint16_t ready[2] = {0};
	uint16_t part[3] = {0};

	part[0] = ( offset & 0b111111111110 ) >> 1;
	part[1] = ( offset >> 12 ) & 0b1111111111;

	part[2] = ( offset >> 22 ) & 0b11;

	if( !S )
	{
		part[2]^= 0b11;
	}

	ready[0] = 0b1111000000000000;
	ready[1] = 0b1101000000000000;

	ready[1]|= part[0];
	ready[0]|= part[1];

	ready[1]|= ( part[2] & 0b1 ) << 11;
	ready[1]|= ( part[2] & 0b10 ) << 13;

	ready[0]|= S << 10;

	return *((uint32_t*) ready );
}

uint16_t emit_blx( int regist )
{
	uint16_t ready = 0b0100011110000000;
	regist&= 0b1111;

	ready|= regist << 3;

	return ready;
}

//0-7 = R0-R7 registr, 8 = LR
uint16_t emit_push( int registr )
{
	uint16_t ready = 0b1011010000000000;
	ready|= 1 << registr;

	return ( ready );
}

uint16_t emit_pop( int registr )
{
	uint16_t ready = 0b1011110000000000;
	ready|= 1 << registr;

	return ( ready );
}

//offset = {0,1020}
uint16_t emit_ldr_short( int offset, int registr )
{
	uint16_t ready = 0b0100100000000000;
	registr&= 0b111;

	ready|= (uint8_t)(offset >> 2);

	ready|= registr << 8;

	return ( ready );
}

uint32_t emit_ldr_long( uint16_t offset, uint8_t registr, uint8_t add )
{
	/*uint32_t ready = 0b1111100011011111;

	if( offset < 0 )
	{
		ready&= ~((uint32_t)( 1 << 7 ));
		offset*= -1;
	}

	ready = ready << 3;
	ready|= ( registr & 0b1111 );

	ready = ready << 12;
	ready|= ( offset & 0b111111111111 );

	return ( ready );*/
	uint16_t part[2];

	part[0] = 0b1111100011011111;
	part[1] = offset & 0b111111111111;
	part[1]|= ( registr & 0b111 ) << 12;

	part[0]|= ( add & 0b1 ) << 7;

	uint32_t ready = *((uint32_t*)part);

	return ready;
}

uint16_t emit_mov( int sourse, int target )
{
	//uint16_t ready = 0;

	//sourse&= 0b111;
	//target&= 0b111;

	//ready = target;
	//ready|= sourse << 3;

	uint16_t ready = 0b0100011000000000;

	ready|= target & 0b111;
	ready|= ( sourse & 0b1111 ) << 3;

	return ( ready );
}

uint16_t emit_cmp( uint8_t first, uint8_t two )
{
	uint16_t ready = 0b0100001010000000;

	ready|= ( first & 0b111 ) << 3;
	ready|= two & 0b111;

	return ready;
}

uint16_t emit_cmp_lt( uint8_t registr, uint8_t value )
{
	uint16_t ready = 0b0010100000000000;

	ready|= ( registr & 0b111 ) << 8;
	ready|= value;

	return ready;
}

uint16_t emit_beq( int16_t offset )
{
	uint16_t ready = 0b1101000000000000;

	ready|= ( offset & 0b111111110 ) >> 1;

	return ready;
}

uint16_t emit_b( uint16_t offset )
{
	uint16_t ready = 0b1110000000000000;

	ready |= ( offset & 0b111111111110 ) >> 1;

	return ready;
}

uint16_t emit_add( uint8_t result, uint8_t two, uint8_t first )
{
	uint16_t ready = 0b0001100000000000;

	ready|= first & 0b111;
	ready|= ( two & 0b111) << 3;
	ready|= ( result & 0b111) << 6;

	return ready;
}
