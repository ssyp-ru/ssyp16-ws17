#include "stack.h"
#include "fault.h"

stack_t datastack,ctrlstack;

cell_t pop()
{
	if (datastack.size>0)
	--datastack.size; else
	fault( "data stack null" );
//	return 0x80000000;
	
	return datastack.a[datastack.size];	
}

void push( cell_t in )
{
	if (datastack.size<STACK_SIZE)
	++datastack.size; else
	fault( "data stack overflow" );
//	datastack.size = 1;

	datastack.a[datastack.size-1] = in;
}

cell_t cpop()
{
	if (ctrlstack.size>0)
	--ctrlstack.size; else
	fault( "ctrl steck null" );
//	return 0x80000000;
	
	return ctrlstack.a[ctrlstack.size];	
}

void cpush( cell_t in )
{
	if (ctrlstack.size<STACK_SIZE)
	++ctrlstack.size; else
	fault( "ctrl steck overflow" );
//	ctrlstack.size = 1;

	ctrlstack.a[ctrlstack.size-1] = in;
}
