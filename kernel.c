#include <stdio.h>
#include "words.h"
#include "fault.h"
#include "forth_simple.h"
#include "stack.h"
#include "string.h"
#include "kernel.h"
#include "user_interface.h"
#include "flash.h"

enum status state = RUN;
char buffer[255], word[64];

void dispatch()
{
	while(1)
	{
		get_user_inputs( buffer );
		while( tokenise( buffer, word ) )
		{
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

typedef enum asm_type_enum
{
	ldr_short_b = 1,
	ldr_short_a = 2,
	ldr_long_b = 3,
	ldr_long_a = 4,
	blx = 5,
	pushs = 6
} asm_type_t;

typedef struct asm_commands_st
{
	asm_type_t commands[128];
	size_t count;
	size_t real_size;
} asm_commands_t;

typedef struct asm_data_st
{
	cell_t data[128];
	size_t count;
} asm_data_t;

typedef struct asm_compiler_st
{
	asm_commands_t asm_commands;
	asm_data_t asm_data;
	char name[64];
} asm_compiler_t;

asm_compiler_t asm_compiler;

#define asm_commands asm_compiler.asm_commands // too many words aaaaaaaaaaa
#define asm_data asm_compiler.asm_data

void compile_handler(char * word)
{
	if( asm_compiler.name[0] == 0 )
	{
		copy( asm_compiler.name, word );
		return;
	}

	func foo = get_word( word );
	if( foo == compile_end )
	{
		foo();
		return;
	}

	if( foo != NULL )
	{
		asm_commands.commands[asm_commands.count] = ldr_long_b;
		asm_commands.commands[asm_commands.count+1] = blx;

		asm_commands.count+= 2;

		asm_data.data[asm_data.count] = foo;

		asm_data.count+= 1;

		asm_commands.real_size+= 6;
	}
	else
	{
		asm_commands.commands[asm_commands.count] = ldr_long_b;
		asm_commands.commands[asm_commands.count+1] = ldr_long_a;
		asm_commands.commands[asm_commands.count+2] = blx;

		asm_commands.count+= 3;

		int num;

		num = string_to_int( word );

		asm_data.data[asm_data.count] = &push;
		asm_data.data[asm_data.count+1] = num;

		asm_data.count+= 2;

		asm_commands.real_size+= 10;
	}
}

void define()
{
	state = COMPILE;
	asm_commands.count = 0;
	asm_data.count = 0;
	asm_commands.real_size = 0;
	asm_compiler.name[0] = 0;
}

uint16_t bin[256];

void compile_end()
{
	uint32_t buffer_32b;
	uint16_t data_pos = 0;
	uint16_t data_offset;
	int pos = 1;

	int registr = 0;

	asm_commands.real_size+= 2;

	bin[0] = emit_push(8);

	for( int i = 0; i < asm_commands.count; i++ )
	{
		registr = 0;

		switch (asm_commands.commands[i])
		{
		case ldr_long_b:
			registr = 2;
		case ldr_long_a:
			data_offset = (asm_commands.real_size - (pos*2)) + ( 4 * data_pos );

			if( pos % 2 == 0 )
			{
				data_offset-= 2;
			}

			buffer_32b = emit_ldr_long( data_offset, registr, 1 );

			data_pos++;

			bin[pos] = buffer_32b;
			bin[pos+1] = buffer_32b >> 16;

			pos+= 2;

			break;
		/*case ldr_long_a:
			data_offset = (asm_commands.real_size - (pos*2)) + ( 4 * data_pos );
			buffer_32b = emit_ldr_long( data_offset, 0, 1 );

			data_pos++;

			bin[pos] = buffer_32b;
			bin[pos+1] = buffer_32b >> 16;

			pos+= 2;

			break;*/
		case ldr_short_b:
			registr = 2;
		case ldr_short_a:
			data_offset = (asm_commands.real_size - (pos*2)) + ( 4 * data_pos );

			bin[pos] = emit_ldr_short( data_offset, registr );

			data_pos++;
			pos++;

			break;
		case blx:

			bin[pos] = emit_blx( 2 );
			pos++;

			break;
		};
	}

	bin[pos] = emit_pop(8);

	pos++;

	for( int i = 0; i < asm_data.count; i++ )
	{
		bin[pos] = asm_data.data[i];
		bin[pos+1] = (asm_data.data[i]) >> 16;

		pos+= 2;
	}

	uintptr_t flash_ptr = where_write( pos * 2 );
	add_word( asm_compiler.name, ((func)flash_ptr)+1 );

	//flash_free = flash_have_memory( pos * 2 );
	flash_write( bin, pos / 2, flash_ptr );

	state = RUN;
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
