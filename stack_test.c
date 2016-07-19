#include <assert.h>
#include <stdio.h>
#include "stack.h"

int main()
{
	push(12);
	assert(pop()==12);
	for(int i = 0; i < 1021; i++)
	{
		push(8);
	}

	while( 1 )
	{
		pop();
	}

	return 0;
}
