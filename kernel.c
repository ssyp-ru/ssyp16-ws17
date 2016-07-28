#include <stdio.h>
#include "words.h"
#include "fault.h"
#include "forth_simple.h"
#include "stack.h"
#include "string.h"
#include "kernel.h"
#include "user_interface.h"
#include "flash.h"
#include "asm_compiler.h"

enum status state = RUN;
char buffer[255], word[64];

void dispatch()
{
	while(1)
	{
		get_user_inputs( buffer );
		while( tokenise( buffer, word ) )
		{
			if(word == NULL)
				break;

			to_lower(word);

			switch(state)
			{
				case RUN: run_handler(word); break;
				case COMPILE: compile_handler(word); break;
				case R_DUMMY: r_dummy_handler(word); break;
				case C_DUMMY: c_dummy_handler(word); break;

			}
		}

		tokenise( NULL, NULL );
	}
}

void r_dummy_handler(char * word)
{
	if (*word == ')')
	{
		state = RUN;
	}
}

void print_bad_word( char *word )
{
	UART_print( ">>>> " );
	UART_print(	word );
	UART_print( " <<<<\r\n" );
	UART_print( "unknown command\r\n" );
}

void run_handler(char * word)
{
	func word_a = get_word( word, RUN );
	if(word_a == NULL)
	{
		if( !check_num( word ) )
		{
			print_bad_word( word );
			//*buffer = 0;
			//tokenise( NULL, NULL );
			return;
		}
		int num;
		num = string_to_int( word );
		push (num);
	}
	else
	{
		word_a();
	}
}
