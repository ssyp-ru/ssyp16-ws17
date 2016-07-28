#include <stdio.h>
#include "stack.h"
#include "words.h"
#include "forth_simple.h"
#include "user_interface.h"
#include "forth_compiler.h"
#include "string.h"
#include "asm_compiler.h"

#define NOP 0xBF00

typedef enum asm_type_enum
{
	ldr_short_b,
	ldr_short_a,
	ldr_long_b,
	ldr_long_a,
	ldr_long_s,
	blx,
	pushs,
	cmpd,
	move_r,
	beq,
	b,
	nope,
	move_a,
	add
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

typedef struct asm_literal_st
{
	char lirterals[8][64];
	size_t count;
	int can_add;
} asm_literal_t;

typedef struct asm_compiler_st
{
	asm_commands_t asm_commands;
	asm_data_t asm_data;
	asm_offset_t asm_offset;
	asm_literal_t asm_literal;
	int compile_time;
	char name[64];
} asm_compiler_t;

asm_compiler_t asm_compiler;

#define asm_commands asm_compiler.asm_commands // too many words aaaaaaaaaaa
#define asm_data asm_compiler.asm_data
#define asm_offset asm_compiler.asm_offset
#define asm_literal asm_compiler.asm_literal

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
	if( foo == parentheses )foo();
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
		if( !check_num( word ) )
		{
			state = RUN;
			print_bad_word( word );
			*buffer = 0;
			tokenise( NULL, NULL );
			return;
		}

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
	asm_literal.count = 0;
	asm_compiler.compile_time = 0;
}

void define_compile_time()
{
	define();
	asm_compiler.compile_time = 1;
}

void compile_end()
{
	uint16_t bin[256];
	uint32_t buffer_32b;
	uint16_t data_pos = 0;
	uint16_t data_offset;
	int pos = 1;
	int cpos = 0;

	uint32_t sum_size = asm_commands.real_size + (asm_data.count*4) + (asm_literal.count*64) + 4;

	uintptr_t flash_pos = where_write_code( sum_size );

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
		case ldr_long_s:
			data_offset = sum_size - (pos*2) - 2;

			if( pos % 2 == 0 )
			{
				data_offset-= 2;
			}

			buffer_32b = emit_ldr_long( data_offset, 2, 1 );

			bin[pos] = buffer_32b;
			bin[pos+1] = buffer_32b >> 16;

			pos+= 2;

			break;
		case blx:

			bin[pos] = emit_blx( 2 );
			pos++;

			break;
		case nope:

			bin[pos] = 0xBF00;
			pos++;

			break;
		case add:

			bin[pos] = emit_add( 0, 1, 0 );
			pos++;

			break;
		case move_a:

			bin[pos] = emit_mov( 15, 1 );
			pos++;

			asm_data.data[data_pos] = (asm_commands.real_size - (pos*2)) +
										(asm_data.count * 4) +
										(asm_data.data[data_pos] * 64);

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

	bin[pos] = flash_pos + 1;
	bin[pos+1] = flash_pos >> 16;

	pos+=2;

	for( int i = 0; i < asm_literal.count; i++ )
	{
		for( int j = 0; j < 32; j++ )
		{
			bin[pos] = *((uint16_t*)(&asm_literal.lirterals[i][j * 2]));
			pos++;
		}
	}

	//uintptr_t flash_pos = where_write_code( pos / 2 );

	flash_write_code( bin, (pos+1) / 2 );
	flash_pos;
	add_word( asm_compiler.name, ((func)flash_pos)+1, asm_compiler.compile_time );


	state = RUN;
}

void forth_recurse()
{
	asm_commands.commands[asm_commands.count] = ldr_long_s;
	asm_commands.commands[asm_commands.count+1] = blx;

	asm_commands.count+= 2;
	asm_commands.real_size+= 6;
}

void c_dummy_handler(char * word)
{
	if(*word == ')')
	{
		state = COMPILE;
	}
}

void literul_print( char *text )
{
	UART_print( text );
	UART_putc( ' ' );
}



int literul_handler(char * word)
{
	if(*word == '"')
	{

		asm_commands.commands[asm_commands.count] = ldr_long_b;
		asm_commands.commands[asm_commands.count+1] = blx;

		asm_commands.count+= 2;

		asm_data.data[asm_data.count] = &UART_next_line;

		asm_data.count+= 1;

		asm_commands.real_size+= 6;

		asm_literal.can_add = 0;

		return 0;
	}
	else
	{
		if( asm_literal.count > 0 && len( asm_literal.lirterals[asm_literal.count-1] ) + len( word ) < 62 && asm_literal.can_add )
		{
			int literal_len = len(asm_literal.lirterals[asm_literal.count-1]);
			*(asm_literal.lirterals[asm_literal.count-1] + literal_len) = ' ';
			literal_len++;
			copy( (asm_literal.lirterals[asm_literal.count-1] + literal_len), word );
			return;
		}

		asm_literal.can_add = 1;

		copy( asm_literal.lirterals[asm_literal.count], word );
		asm_literal.count++;

		asm_commands.commands[asm_commands.count] = ldr_long_b;
		asm_commands.commands[asm_commands.count+1] = move_a;
		asm_commands.commands[asm_commands.count+2] = ldr_long_a;
		asm_commands.commands[asm_commands.count+3] = add;
		asm_commands.commands[asm_commands.count+4] = blx;

		asm_commands.count+= 5;
		asm_commands.real_size+= 14;

		asm_data.data[asm_data.count] = &literul_print;
		asm_data.data[asm_data.count+1] = asm_literal.count-1;

		asm_data.count+= 2;

		return 1;
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

	if( offset > 0 )
	{
		offset*= -1;
	}

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
