#include <stdio.h>
#include "words.h"
#include "fault.h"
#include "forth_simple.h"
#include "stack.h"
#include "string.h"
#include "kernel.h"
#include "user_interface.h"

enum status state = RUN;
char buffer[255], word[64];

void dispatch()
{
	while(1)
	{
		get_user_inputs( buffer );
		while(1)
		{
			tokenise( buffer, word );
			if(word == NULL)
				break;
			switch(state)
			{
				case RUN: run_handler(word); break;
				case COMPILE: compile_handler(word); break;
				case R_DUMMY: r_dummy_handler(word); break;
				case C_DUMMY: c_dummy_handler(word); break;
				case LITERAL: literul_handler(word); break;

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

void run_handler(char * word)
{
	func word_a = get_word(word);
	if(word_a == NULL)
	{
		int num;
		num = string_to_int( word );
		push (num);
	}
	else
	{
		word_a();
	}
}

void compile_handler(char * word)
{
	
}

void c_dummy_handler(char * word)
{
	if(*word == ')')
	{
		state = COMPILE;
	}
}

void literul_handler(char * word)
{
	if(*word == '"')
	{
		state = COMPILE;
	}

}
