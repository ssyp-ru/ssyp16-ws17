#include <stdint.h>
#include <string.h>

#ifndef __STACK__
#define __STACK__

	#define STACK_SIZE 1024

	typedef intptr_t cell_t;
 
	typedef struct {
		size_t size;
		cell_t data[STACK_SIZE]; 
	} stack_t;
	extern stack_t datastack,ctrlstack;

	void push( cell_t new_element );
	cell_t pop();

	void cpush( cell_t new_element );
	cell_t cpop();

#endif
