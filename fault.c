#include <stdlib.h>
#include <stdio.h>
#include "fault.h"
#include "kernel.h"
#include "words.h"
#include "user_interface.h"

extern void ResetISR(); // bad magic

void restart() // good magic
{
	*((uint32_t*)(0xE000E000+0xD0C)) = 0x05FA0000 + 0b101;//do magic
}

extern void main();

void drop_to_main()
{
	state = RUN;
	uint16_t bin[8];
	uint32_t ldr = emit_ldr_long( 4, 2, 1 );

	bin[0] = ldr;
	bin[1] = ldr >> 16;
	bin[2] = emit_blx(2);
	bin[3] = 0;
	bin[4] = (&main)+0xDE;
	bin[5] = 0;

	(((func)bin)+1)();
}

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
			restart();
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
