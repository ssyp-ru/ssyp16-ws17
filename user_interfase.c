#include "user_interface.h"
#include <stdio.h>

const void *RCGCUART = 0x400FE000+0x618;
const void *RCGCGPIO = 0x400FE000+0x608;

#define __GPIOBASE 0x40004000

const void *GPIOBASE = __GPIOBASE;

const void *GPIOPA   = __GPIOBASE+0x420;
const void *GPIO2MA  = __GPIOBASE+0x500;
const void *GPIOSRCS = __GPIOBASE+0x518;
const void *GPIOPC   = __GPIOBASE+0x52C;
const void *GPIOCR   = __GPIOBASE+0x524;

#define __UARTBASE 0x4000C000

const void *UARTBASE = __UARTBASE;

const void *UARTLC   = __UARTBASE+0x02C;
const void *UARTC    = __UARTBASE+0x030;
const void *UARTFLAG = __UARTBASE+0x018;
const void *UARTCC   = __UARTBASE+0xFC8;

const void *UARTIBRD = __UARTBASE+0x024;
const void *UARTFBRD = __UARTBASE+0x028;

const char UART_PUTC_FLAG = ( 0 | ( 1 << 5 ) ) | ( 1 << 3 );

void flag_on( void *ptr_byte, int pos )
{
	uint16_t buffer = *((uint16_t *)ptr_byte);
	buffer = buffer | (1 << pos);
	*((uint16_t*)ptr_byte) = buffer;
}

void flag_off( void *ptr_byte, int pos )
{
	uint16_t buffer = *((uint16_t *)ptr_byte);
	buffer = buffer & ~(1 << pos);
	*((uint16_t*)ptr_byte) = buffer;
}

void initUserInterfase()
{
// 1 << 16
// 0xF000 >> 16
// a & 0xFFF0
// a | (1 << 16)
// ^
// ~
	flag_on( RCGCUART, 0 );
	flag_on( RCGCGPIO, 0 );

	flag_on( GPIOPA,   0 );
	flag_on( GPIO2MA,  0 );
	flag_on( GPIOSRCS, 0 );

	flag_on( GPIOPC, 0 );
	flag_on( GPIOPC, 1 );
	flag_on( GPIOPC, 2 );
	flag_on( GPIOPC, 3 );
	
	//conf
	flag_off( UARTC, 0 ); // UART OFF

	flag_on( UARTLC, 8 ); // Transmit Enable
	flag_on( UARTLC, 9 ); // Receive  Enable

	flag_on( UARTLC, 5 ); // 8-bit mode
	flag_on( UARTLC, 6 ); // 8-bit mode
	//flag_on( UARTLC, 4 );

	*(uint32_t*)UARTIBRD = 104;
	*(uint32_t*)UARTFBRD = (int)(0.16666 * 64 + 0.5);

	//*(uint32_t*)UARTCC   = 0x5;
	//flag_off( UARTC, 4 );
	//flag_off( UARTC, 5 );

	flag_on( UARTC, 0 ); // UART ON

	//flag_off( UARTC, 4 );
	//flag_off( UARTC, 5 );
}

void getUserInput( char *buffer )
{
	char inChar = getchar();
	int count = 0;	

	while( inChar != 10 )
	{
		buffer[count] = inChar;
		count++;

		if( count > 126 )
		{
			break;
		}

		inChar = getchar();
	}

	buffer[count] = 0;
}

void UART_print( char *text )
{
	while( *text != 0 )
	{
		*(char *)UARTBASE = *text;

		text++;
	}
}

void UART_putc( char c )
{
	//flag_off( UARTC, 4 );
	//flag_off( UARTC, 5 );


	char data;
	do
	{
		data = *(char *)UARTFLAG;
	}
	while( data & ( 1 << 5 ) );

	*(char  *)UARTBASE = c;
}
