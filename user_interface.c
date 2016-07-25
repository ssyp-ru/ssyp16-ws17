#include "user_interface.h"
#include <stdio.h>

const void *RCGCUART = 0x400FE000+0x618;
const void *RCGCGPIO = 0x400FE000+0x608;

#define __GPIOBASE 0x40004000

const void *GPIOBASE = __GPIOBASE;

<<<<<<< HEAD
const void *GPIOAFSEF   = __GPIOBASE+0x420;
const void *GPIO2MA  = __GPIOBASE+0x500;
const void *GPIOSRCS = __GPIOBASE+0x518;
const void *GPIOPC   = __GPIOBASE+0x52C;
const void *GPIOCR   = __GPIOBASE+0x524;
const void *GPIODEN  = __GPIOBASE+0x51C;
const void *GPIOPDR  = __GPIOBASE+0x510;
const void *GPIOPUR  = __GPIOBASE+0x514;
=======
const void *GPIOAFSEF= __GPIOBASE+0x420;
const void *GPIOPCTL = __GPIOBASE+0x52C;
const void *GPIODEN  = __GPIOBASE+0x51C;
const void *GPIOLOCK = __GPIOBASE+0x520;
>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc

#define __UARTBASE 0x4000C000

const void *UARTBASE = __UARTBASE;

<<<<<<< HEAD
const void *UARTLC   = __UARTBASE+0x02C;
const void *UARTC    = __UARTBASE+0x030;
const void *UARTFLAG = __UARTBASE+0x018;
=======
const void *UARTLCRH = __UARTBASE+0x02C;
const void *UARTCTL  = __UARTBASE+0x030;
const void *UARTFR   = __UARTBASE+0x018;
>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
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

<<<<<<< HEAD
//set bit(with num pos) on ptr_byte 1
void flag_on( void *ptr_byte, int pos )
{
	*((uint32_t *)ptr_byte) = (*((uint32_t *)ptr_byte)) | ( 1 << pos );
=======
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
>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
}

//set bit(with num pos) on ptr_byte 0
void flag_off( void *ptr_byte, int pos )
{
<<<<<<< HEAD
	*((uint32_t *)ptr_byte) = (*((uint32_t *)ptr_byte)) & ~(1 << pos);
=======
	*((uint32_t *)ptr_byte) ^= (1 << pos);
>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
}

void init_UART()
{
	//init GPIO and set on UART
	flag_on( RCGCUART, UART_A );
	flag_on( RCGCGPIO, UART_A );

<<<<<<< HEAD
	*(uint32_t*)(GPIOBASE+0x520) = 0x0;

	for( uint64_t i = 0; i < 250000; i++ ){;}
=======
	*(uint32_t*)GPIOLOCK = 0;
>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc

	flag_on( GPIODEN, PIN_1 );
	flag_on( GPIODEN, PIN_2 );

	flag_on( GPIOAFSEF, PIN_1 );
	flag_on( GPIOAFSEF, PIN_2 );

<<<<<<< HEAD
	flag_on( GPIOPC, PIN_1 * 4 );
	flag_on( GPIOPC, PIN_2 * 4 );

	for( uint64_t i = 0; i < 250000; i++ ){;}

	//conf
	flag_off( UARTC, UART_CONTROL_ON_F ); // UART OFF

	flag_on( UARTC, UART_CONTROL_TRANSMIT_F ); // Transmit Enable
	flag_on( UARTC, UART_CONTROL_RECEIVE_F ); // Receive  Enable

	flag_on( UARTLC, UART_CONTROL_8_BIT_F ); // 8-bit mode
	flag_on( UARTLC, UART_CONTROL_8_BIT_F+1 ); // 8-bit mode
	*(uint32_t*)UARTCC = 5;

	*(uint32_t*)UARTIBRD = 104;										//SETUP clock
	*(uint32_t*)UARTFBRD = (int)(0.1666666 * 64 + 0.5);				//SETUP clock

	*(uint16_t*)UARTC = 0x301; // set uart on, receive on, transmit on

	for( uint64_t i = 0; i < 250000; i++ ){;}
}

void get_user_input( char *buffer )
=======
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

void get_user_input( char *line )
>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
{
	char inChar = UART_getc();
	int count = 0;	

<<<<<<< HEAD
	while( inChar != 0xD && inChar != 0x0 )
	{
		UART_putc( inChar );
		buffer[count] = inChar;
=======
	while( inChar != CHAR_ENTER && inChar != CHAR_EOF )
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

		line[count] = inChar;
>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
		count++;

		if( count > 126 )
		{
			break;
		}

		inChar = UART_getc();
	}

	UART_print( "\r\n" );

<<<<<<< HEAD
	buffer[count] = 0;
=======
	line[count] = 0;
>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
}

char UART_getc()
{
	char data;
	do
	{
<<<<<<< HEAD
		data = *(char *)UARTFLAG;
=======
		data = *(char *)UARTFR;
>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
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
<<<<<<< HEAD
		data = *(char *)UARTFLAG;
=======
		data = *(char *)UARTFR;
>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
	}
	while( data & UART_PUTC_FLAG ); // wait UART

	*(char  *)UARTBASE = c;
}
