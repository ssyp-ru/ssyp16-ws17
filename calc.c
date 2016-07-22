#include "user_interfase.h"
#include "string.h"
#include "stack.h"
#include "words.h"
#include "charb.h"
#include <stdio.h>

int main()
{
	initWords();
	
	char input[128];// = "32 -11 add print dup add print -99 add print 0 mul print drop  print";
	func foo;
	char token[33];
	while(1)
	{
		getUserInput( input );

		while( tokenise( input, token ) )
		{
			foo = get_word(token);
			if( foo != 0 )
			{
				foo();
			}
			else
			{
				push( charToInt(token) );
			}
		}
	}

	return 0;
}
