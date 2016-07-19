#include "words.h"
#include "stack.h"
#include "fault.h"
#include "forth_simple.h"
#include <stdio.h>

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
	push( pop() / tmp );
}

void forth_mul()
{
	push( pop() * pop() );
}

void forth_mod()
{
	cell_t tmp = pop();
	push( pop() % tmp );
}

void forth_divmod()
{
	cell_t first = pop();
	cell_t last = pop();

	push( last / first );
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

	push( last  );
	push( first );
	push( midle );
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

void forth_print()
{
	if( datastack.size < 1 )
	{
		printf( "No Have value in stack\n" );
	}
	else
	{
		printf( "%d\n", (int)datastack.data[ datastack.size-1 ] );
	}
}
