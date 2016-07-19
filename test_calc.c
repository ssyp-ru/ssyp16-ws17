#include "words.h"
#include "stack.h"
#include "forth_simple.h"
#include <stdio.h>
#include <assert.h>

void call( func foo )
{
	foo();
}

int main()
{
	initWords();
	
	push(2);
	push(3);

	call(get_word("add"));

	call(get_word("print"));

	push(5);

	call( get_word("and") );
	call( get_word("print") );	

	call( get_word("dup") );

	call( get_word("add") );
	call( get_word("print") );

	call( get_word("drop") );
	call( get_word("print") );
	call( get_word("drop") );

	return 0;
}
