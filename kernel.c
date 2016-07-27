#include <stdio.h>
#include "words.h"
#include "fault.h"
#include "forth_simple.h"
#include "stack.h"
#include "string.h"
#include "kernel.h"
#include "user_interface.h"
#include "flash.h"
#include "asm_compiler.h"

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
	func word_a = get_word( word, RUN );
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
	pushs = 6,
	cmpd = 7,
	move_r = 8,
	beq = 9,
	b = 10,
	nope = 11
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

typedef struct asm_offset_st
{
	cell_t offset[64];
	size_t count;
} asm_offset_t;

typedef struct asm_compiler_st
{
	asm_commands_t asm_commands;
	asm_data_t asm_data;
	asm_offset_t asm_offset;
	char name[64];
} asm_compiler_t;

asm_compiler_t asm_compiler;

#define asm_commands asm_compiler.asm_commands // too many words aaaaaaaaaaa
#define asm_data asm_compiler.asm_data
#define asm_offset asm_compiler.asm_offset

void compile_handler(char * word)
{
	if( asm_compiler.name[0] == 0 )
	{
		copy( asm_compiler.name, word );
		return;
	}

	func foo = get_word( word, COMPILE );
	if( foo != 0 )
	{
		foo();
		return;
	}

	foo = get_word( word, RUN );

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
	asm_offset.count = 0;
}

void compile_end()
{
	uint16_t bin[256];
	uint32_t buffer_32b;
	uint16_t data_pos = 0;
	uint16_t data_offset;
	int pos = 1;
	int cpos = 0;

	int registr = 0;

	asm_commands.real_size+= 2;

	bin[0] = emit_push(8);

	for( int i = 0; i < asm_commands.count; i++ )
	{
		registr = 0;

		switch (asm_commands.commands[i])
		{
		case cmpd:
			bin[pos] = emit_cmp_lt( 0, 0 );
			pos++;

			break;
		case beq:
			bin[pos] = emit_beq( asm_offset.offset[cpos] );
			pos++;
			cpos++;

			break;
		case b:
			bin[pos] = emit_b( asm_offset.offset[cpos] - 2 );
			pos++;
			cpos++;

			break;
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
		case nope:

			bin[pos] = 0xBF00;
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

	//uintptr_t flash_pos = where_write_code( pos / 2 );

	uintptr_t flash_pos = flash_write_code( bin, (pos+1) / 2 );
	add_word( asm_compiler.name, ((func)flash_pos)+1, 0 );


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

void forth_do()
{
	asm_commands.commands[asm_commands.count] = ldr_long_b;
	asm_commands.commands[asm_commands.count+1] = blx;
	asm_commands.commands[asm_commands.count+2] = ldr_long_b;
	asm_commands.commands[asm_commands.count+3] = blx;
	asm_commands.commands[asm_commands.count+4] = ldr_long_b;
	asm_commands.commands[asm_commands.count+5] = blx;
	asm_commands.commands[asm_commands.count+6] = ldr_long_b;
	asm_commands.commands[asm_commands.count+7] = blx;
	asm_commands.commands[asm_commands.count+8] = ldr_long_b;
	asm_commands.commands[asm_commands.count+9] = blx;

	asm_commands.count+= 10;
	asm_commands.real_size+= 30;

	asm_data.data[asm_data.count] = &pop;
	asm_data.data[asm_data.count+1] = &cpush;
	asm_data.data[asm_data.count+2] = &pop;
	asm_data.data[asm_data.count+3] = &cpush;
	asm_data.data[asm_data.count+4] = &forth_cswap;

	asm_data.count+= 5;

	cpush( asm_commands.real_size );
	cpush( 3 );
}

void forth_loop()
{
	asm_commands.commands[asm_commands.count] = ldr_long_b;			//4
	asm_commands.commands[asm_commands.count+1] = blx;				//22
	asm_commands.commands[asm_commands.count+2] = cmpd;				//20
	asm_commands.commands[asm_commands.count+3] = beq;				//18
	asm_commands.commands[asm_commands.count+4] = ldr_long_b;		//8
	asm_commands.commands[asm_commands.count+5] = blx;				//16
	asm_commands.commands[asm_commands.count+6] = ldr_long_b;		//12
	asm_commands.commands[asm_commands.count+7] = blx;				//14

	asm_commands.count+= 8;
	asm_commands.real_size+= 22;

	asm_data.data[asm_data.count] = &cloop;
	asm_data.data[asm_data.count+1] = &cpop;
	asm_data.data[asm_data.count+2] = &cpop;
	asm_data.count+= 3;

	cpop();
	int pos = cpop();

	cell_t offset = pos - asm_commands.real_size + 10;
	asm_offset.offset[asm_offset.count] = offset;

	asm_offset.count++;
}

void forth_if()
{
	asm_commands.commands[asm_commands.count] = ldr_long_b;
	asm_commands.commands[asm_commands.count+1] = blx;
	asm_commands.commands[asm_commands.count+2] = cmpd;
	asm_commands.commands[asm_commands.count+3] = beq;
	asm_commands.commands[asm_commands.count+4] = nope;

	asm_commands.count+= 5;
	asm_commands.real_size+= 12;

	asm_data.data[asm_data.count] = &pop;

	asm_data.count++;

	asm_offset.offset[asm_offset.count] = asm_commands.real_size;

	cpush( asm_offset.count );
	cpush( 0 );

	asm_offset.count++;
}

void forth_else()
{
	cpop();
	int pos = cpop();
	cell_t offset = asm_commands.real_size - asm_offset.offset[pos];

	asm_offset.offset[pos] = offset + 2;
	asm_offset.offset[asm_offset.count] = asm_commands.real_size + 2;
	asm_offset.count++;

	cpush( asm_offset.count-1 );
	cpush( 1 );

	asm_commands.commands[asm_commands.count] = b;
	asm_commands.count++;
	asm_commands.real_size+= 2;
}

void forth_then()
{
	cpop();
	int pos = cpop();
	cell_t offset = asm_commands.real_size - asm_offset.offset[pos];

	asm_offset.offset[pos] = offset;
}
