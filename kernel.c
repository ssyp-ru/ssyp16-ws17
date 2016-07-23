#include <stdio.h>
#include "words.h"
#include "fault.h"
#include "forth_simple.h"
#include "charb.h"
#include "stack.h"
#include "string.h"
#include "kerlen.h"
enum status
{
	RUN	= 0,
	COMPILE	= 1,
	R_DUMMY = 2,
	C_DUMMY = 3,
	LITERAL = 4
};

static enum status state = RUN;
static char[255] buffer, word;

void dispatch()
{
	while(1)
	{
		scanf("%s/n", & buffer);
		while(1)
		{
			word =  tokenise(buffer);
			if(word = NULL)
				break;
			switch(state)
			{
				case RUN: run_handler(&word) break;
				case COMPLITE: compile_handler(&word) break;
				case R_DUMMY: r_dummy_handler(&word) break;
				case C_DUMMY: c_dummy_handler(&word) break;
				case LITERUL: literul_handler(&word) break;

			}
		}
	}
}

void r_dummy_handler(char * word)
{
	if (*word == ')')
	{
		status = RUN;
	}
}

void run_handler(char * word)
{
	func word_a =  get_word(word);
	if(word_a == NULL)
	{
		if(((*word >= '0') && (* word <= '9')) || (*word == '-'))
		{
			int num;
			dasha_atoi(& num, word);
			push (num);
		}
		else
		{
			printf("Unknown word.");
		}
	}
}

void compile_handler(char * word)
{
	
}

void c_dummy_handler(char * word)
{
	if(*word == ')')
	{
		status = COMPILE;
	}
}

void literul_handler(char * word)
{
	if(*word == '"')
	{
		status = COMPILE;
	}

}
int main()
{
	return 0;
}
