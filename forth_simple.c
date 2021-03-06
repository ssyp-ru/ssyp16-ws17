#include "words.h"
#include "stack.h"
#include "fault.h"
#include "forth_simple.h"
#include "user_interface.h"
#include "kernel.h"
#include <stdio.h>

void parentheses()
{
	//if (state == RUN)
	//	state = R_DUMMY;
	//else
	//	state = C_DUMMY;
	while(1)
	{
		tokenise( buffer, word );
		if( word == NULL || !r_dummy_handler( word ) )
		{
			return;
		}
	}
}

void quote()
{
	//state = LITERAL;

	while(1)
	{
		tokenise( buffer, word );
		if( word == NULL || !literul_handler( word ) )
		{
			return;
		}
	}
}

void forth_add()
{
	push( pop() + pop() );
}

void forth_sub()
{
	cell_t tmp = pop();
	push( pop() - tmp );
}

void forth_div()
{
	cell_t tmp = pop();
	if( tmp == 0 )
	{
		fault( "division by zero" );
	}
	push( (cell_t)(pop() / tmp) );
}

void forth_mul()
{
	push( pop() * pop() );
}

void forth_mod()
{
	cell_t tmp = pop();
	if( tmp == 0 )
	{
		fault( "division by zero" );
	}
	push( pop() % tmp );
}

void forth_divmod()
{
	cell_t first = pop();
	cell_t last = pop();

	if( last == 0 )
	{
		fault( "division by zero" );
	}

	push( (cell_t)(last / first) );
	push( last % first );	
}

void forth_swap()
{
	if( datastack.size < 2 )
	{
		fault( "data stack too short" );
	}

	cell_t first = pop();
	cell_t last  = pop();

	push( first );
	push( last );
}

void forth_cswap()
{
	if( ctrlstack.size < 2 )
	{
		fault( "ctrl stack too short" );
	}

	cell_t first = cpop();
	cell_t last  = cpop();

	cpush( first );
	cpush( last );
}

void forth_dup()
{
	cell_t tmp = pop();

	push( tmp );
	push( tmp );
}

void forth_cdup()
{
	cell_t tmp = cpop();

	cpush( tmp );
	cpush( tmp );
}

void forth_rot()
{
	cell_t first = pop();
	cell_t midle = pop();
	cell_t last  = pop();

	push( midle  );
	push( first );
	push( last );
}

void forth_crot()
{
	cell_t first = cpop();
	cell_t midle = cpop();
	cell_t last  = cpop();

	cpush( last  );
	cpush( midle );
	cpush( first );
}

void forth_drop()
{
	pop();
}

void forth_cdrop()
{
	cpop();
}

void forth_and()
{
	push( (pop() && pop()) ? 1 : 0 );
}

void forth_or()
{
	push( (pop() || pop()) ? 1 : 0 );
}

void forth_low()
{
	push( (pop() > pop()) ? 1 : 0 );
}

void forth_eq()
{
	push( (pop() == pop()) ? 1 : 0 );
}

void forth_hight()
{
	push( (pop() < pop()) ? 1 : 0);
}

void forth_lowe()
{
	push( (pop() >= pop()) ? 1 : 0 );
}

void forth_highte()
{
	push( (pop() <= pop()) ? 1 : 0 );
}

void forth_band()
{
	push( (pop() & pop()) ? 1 : 0 );
}

void forth_bor()
{
	push( (pop() | pop()) ? 1 : 0 );
}

void forth_over()
{
	cell_t buffer[2];
	buffer[0] = pop();
	buffer[1] = pop();
	push( buffer[1] );
	push( buffer[0] );
	push( buffer[1] );
}

void forth_print()
{
	if( datastack.size < 1 )
	{
		UART_print( "stack is empty\r\n" );
	}
	else
	{
		char value[16];
		//printf( "%d\n", (int)datastack.data[ datastack.size-1 ] );
		Dasha_itoa( (int)pop(), value );
		UART_print( value );
		UART_print( "\r\n" );
	}
}

void forth_print_all()
{
	if( datastack.size < 1 )
	{
		UART_print( "stack is empty\r\n" );
	}
	else
	{
		char value[16];
		for( int i = 0; i < datastack.size; i++ )
		{
			Dasha_itoa( (int)datastack.data[i], value );
			UART_print( value );
			UART_putc( ' ' );
		}
		UART_print( "\r\n" );
	}
}

void forth_setmem()
{
	cell_t *ptr = (cell_t*)pop();
	*ptr = pop();
}

void forth_getmem()
{
	cell_t *ptr = (cell_t*)pop();
	push( *ptr );
}

void forth_i()
{
	if( ctrlstack.size >= 2)
	{
		push( ctrlstack.data[1] );
	}
}

void forth_j()
{
	if( ctrlstack.size >= 4)
	{
		push( ctrlstack.data[3] );
	}
}

void forth_data_to_ctrl()
{
	cpush( pop() );
}

void forth_ctrl_to_data()
{
	push( cpop() );
}
