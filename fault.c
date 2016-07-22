#include <stdlib.h>
#include <stdio.h>
#include "fault.h"

extern void ResetISR();

void fault( char *error )
{
	UART_print( "Program crash with error: " );
	UART_print( error );
	UART_print( "\r\nRestart program(y/n)?\r\n" );

	char input = UART_getc();
	if( input == 'y' || input == 'Y' )
	{
		ResetISR();
	}

	exit( 1 );
}
