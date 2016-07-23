#include <stdio.h>
#include "words.h"
#include "fault.h"
#include "forth_simple.h"
#include "charb.h"
#include "stack.h"
#include "string.h"

enum status { RUN, COMPILE, R_DUMMY, C_DUMMY, LITERUL };

static enum status state = RUN;
static char[255] buffer, word;

void a ()
{
	if (state == RUN)
		state = R_DUMMY; 
	else
		state = C_DUMMY;
}

void b ()
{
	state = LITERUL;
}

void dispatch ()
{
	while (1)
	{
		scanf ("%s/n", & buffer)
		while (1)
		{
			word =  tokenise ( buffer );
			if (word = NULL)
				break;
			switch (state)
			{
				case RUN: run_handler (& word) break;
				case COMPLITE: complite_handler (& word) break;
				case R_DUMMY: r_dummy_hendler (& word) break;
				case C_DUMMY: c_dummy_hendler (& word) break;
				case LITERUL: literul_hendler (& word) break;

			}
		}

}

void r_dummy_handler (char * word)
{
	if (* word == ')')
	{
		status = RUN;
	}
}

void run_handler(char * word)
{
	func word_a =  get_word ( word );
	if (word_a == NULL)
	{
		if (((* word >= '0') && (* word <= '9')) || (* word == '-'))
		{
			int num;
			dasha_atoi (& num, word);
			push (num);
		}
		else
		{
			printf ("Unknown word.");
		}
	}
        
}

void c_dummy_handler (char * word)
{
	if (* word == ')')
	{
		status = COMPILE;
	}
}

void literul_handler(char * word)
{
	if (* word == '"')
	{
		status = COMPILE;
	}

}
int main ()
{
	return 0;
}
