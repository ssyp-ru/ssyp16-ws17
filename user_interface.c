#include "user_interface.h"
#include <stdio.h>

const void *RCGCUART = 0x400FE000+0x618;
const void *RCGCGPIO = 0x400FE000+0x608;

#define __GPIOBASE 0x40004000

const void *GPIOBASE = __GPIOBASE;

const void *GPIOAFSEF= __GPIOBASE+0x420;
const void *GPIOPCTL = __GPIOBASE+0x52C;
const void *GPIODEN  = __GPIOBASE+0x51C;
const void *GPIOLOCK = __GPIOBASE+0x520;

#define __UARTBASE 0x4000C000

const void *UARTBASE = __UARTBASE;

const void *UARTLCRH = __UARTBASE+0x02C;
const void *UARTCTL  = __UARTBASE+0x030;
const void *UARTFR   = __UARTBASE+0x018;
const void *UARTCC   = __UARTBASE+0xFC8;

const void *UARTIBRD = __UARTBASE+0x024;
const void *UARTFBRD = __UARTBASE+0x028;

const int UART_CONTROL_TRANSMIT_F = 8;
const int UART_CONTROL_RECEIVE_F = 9;
const int UART_CONTROL_8_BIT_F = 5;
const int UART_CONTROL_ON_F = 5;

const int PIN_1 = 0;
const int PIN_2 = 1;

const int UART_A = 0;

const char UART_PUTC_FLAG = ( 0 | ( 1 << 5 ) ) | ( 1 << 3 );
const char UART_GETC_FLAG = ( 0 | ( 1 << 4 ) ) | ( 1 << 3 );

const char CHAR_ENTER = 0xD;
const char CHAR_EOF = 0;
const char CHAR_BACKSPACE = 0x7F;

const char PIOSC = 5;

const uint16_t UART_CLOCK_INT = 104;
const uint16_t UART_CLOCK_FLOAT = 11;

const uint16_t UART_ON_RECEIVE_TRANSMIT_F = 0x301;

//set bit(with num pos) on ptr_byte 1
void flag_on( void *ptr_byte, int pos )
{
	*((uint32_t *)ptr_byte) |= ( 1 << pos );
}

//set bit(with num pos) on ptr_byte 0
void flag_off( void *ptr_byte, int pos )
{
	*((uint32_t *)ptr_byte) ^= (1 << pos);
}

void init_UART()
{
	//init GPIO and set on UART
	flag_on( RCGCUART, UART_A );
	flag_on( RCGCGPIO, UART_A );

	*(uint32_t*)GPIOLOCK = 0;

	flag_on( GPIODEN, PIN_1 );
	flag_on( GPIODEN, PIN_2 );

	flag_on( GPIOAFSEF, PIN_1 );
	flag_on( GPIOAFSEF, PIN_2 );

	flag_on( GPIOPCTL, PIN_1 * 4 );
	flag_on( GPIOPCTL, PIN_2 * 4 );

	//conf UART
	flag_off( UARTCTL, UART_CONTROL_ON_F ); // UART OFF

	flag_on( UARTCTL, UART_CONTROL_TRANSMIT_F ); // Transmit Enable
	flag_on( UARTCTL, UART_CONTROL_RECEIVE_F ); // Receive  Enable

	*(uint32_t*)UARTCC = PIOSC;

	*(uint32_t*)UARTIBRD = UART_CLOCK_INT;										//SETUP clock
	*(uint32_t*)UARTFBRD = UART_CLOCK_FLOAT;									//SETUP clock //(int)(0.1666666 * 64 + 0.5);

	flag_on( UARTLCRH, UART_CONTROL_8_BIT_F ); // 8-bit mode
	flag_on( UARTLCRH, UART_CONTROL_8_BIT_F+1 ); // 8-bit mode

	*(uint16_t*)UARTCTL = UART_ON_RECEIVE_TRANSMIT_F; // UART ON
}

void get_user_inputs( char *line )
{
	char inChar = UART_getc();
	int count = 0;	

	while( inChar != CHAR_ENTER && inChar != CHAR_EOF && inChar != '\r' && inChar != '\n' )
	{
		UART_putc( inChar );

		if( inChar == CHAR_BACKSPACE )
		{
			if( count > 2 )
			{
				count--;
			}
			else
			{
				count = 0;
			}
			inChar = UART_getc();
			continue;
		}

		if( inChar == 0x1B || inChar == '\n' )
		{
			inChar = UART_getc();
			continue;
		}

		line[count] = inChar;

		count++;

		if( count > 126 )
		{
			break;
		}

		inChar = UART_getc();
	}

	UART_print( "\r\n" );

	line[count] = 0;
}

char UART_getc()
{
	char data;
	do
	{
		data = *(char *)UARTFR;
	}
	while( data & UART_GETC_FLAG );

	return (*(char*)UARTBASE);
}

void UART_print( char *text )
{
	while( *text != 0 )
	{
		UART_putc( *text );

		text++;
	}
}

void UART_putc( char c )
{
	char data;
	do
	{
		data = *(char *)UARTFR;
	}
	while( data & UART_PUTC_FLAG ); // wait UART

	*(char  *)UARTBASE = c;
}
