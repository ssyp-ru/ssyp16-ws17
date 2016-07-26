#include "stack.h"
#include "fault.h"

stack_t datastack,ctrlstack;

cell_t pop()
{
	if (datastack.size>0)
	--datastack.size; else
	fault( "data stack underflow" );
//	return 0x80000000;
	
	return datastack.data[datastack.size];
}

void push( cell_t in )
{
	if (datastack.size<STACK_SIZE)
	++datastack.size; else
	fault( "data stack overflow" );
//	datastack.size = 1;

	datastack.data[datastack.size-1] = in;
}

cell_t cpop()
{
	if (ctrlstack.size>0)
	--ctrlstack.size; else
	fault( "ctrl steck underflow" );
//	return 0x80000000;
	
	return ctrlstack.data[ctrlstack.size];
}

void cpush( cell_t in )
{
	if (ctrlstack.size<STACK_SIZE)
	++ctrlstack.size; else
	fault( "ctrl steck overflow" );
//	ctrlstack.size = 1;

	ctrlstack.data[ctrlstack.size-1] = in;
}
