#include <stdlib.h>
#include <stdio.h>
#include "fault.h"

extern void ResetISR();

void fault( char *error )
{
	UART_print( "Program crash with error: " );
	UART_print( error );
	UART_print( "\r\nRestart program(y/n)?\r\n" );

	char input;
	while(1)
	{
		input = UART_getc();

		if( input == 'y' || input == 'Y' )
		{
			UART_print( "restart... plase wait... \r\n" );
			ResetISR();
		}
		else if( input == 'n' || input == 'N' )
		{
			break;
		}
		else
		{
			UART_print( "\rPlease enter \'y\' or \'n\'\r\n" );
		}
	}

	while(1){}
}

void system_fault()
{
	fault( "unknow error" );
}
