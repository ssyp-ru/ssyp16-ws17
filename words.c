//#include <string.h>
//#include <stdio.h>
#include "words.h"
#include "fault.h"
#include "forth_simple.h"
#include "kernel.h"
#include "flash.h"
#include "string.h"

#define strlen len
#define strcpy copy
//int cmp( char *leftWord, char *rightWord );

word_mass_t words;

void init_words()
{
	add_word( "+", &forth_add, 0 );
	add_word( "-", &forth_sub, 0 );
	add_word( "/", &forth_div, 0 );
	add_word( "*", &forth_mul, 0 );
	add_word( "%", &forth_mod, 0 );
	add_word( "/%", &forth_divmod, 0 );

	add_word( "swap", &forth_swap, 0 );
	add_word( "dup", &forth_dup, 0 );
	add_word( "rot", &forth_rot, 0 );
	add_word( "drop", &forth_drop, 0 );

	add_word( "cswap", &forth_cswap, 0 );
	add_word( "cdup", &forth_cdup, 0 );
	add_word( "crot", &forth_crot, 0 );
	add_word( "cdrop", &forth_cdrop, 0 );

	add_word( "&&", &forth_and, 0 );
	add_word( "||", &forth_or, 0 );

	add_word( "<", &forth_low, 0 );
	add_word( "<=", &forth_lowe, 0 );

	add_word( ">", &forth_hight, 0 );
	add_word( ">=", &forth_highte, 0 );

	add_word( "==", &forth_eq, 0 );

	add_word( "&", &forth_band, 0 );
	add_word( "|", &forth_bor, 0 );

	add_word( ".", &forth_print, 0 );
	add_word( "(", &parentheses, 0 );
	add_word( "\"", &quote, 0 );

	add_word( ":", &define, 0 );
	add_word( ";", &compile_end, 1 );

	add_word( ".S", &forth_print_all, 0 );

	add_word( "@", &forth_setmem, 0 );
	add_word( "!", &forth_getmem, 0 );

	add_word( "IF", &forth_if, 1 );
	add_word( "ELSE", &forth_else, 1 );
	add_word( "THEN", &forth_then, 1 );

	add_word( "DO", &forth_do, 1 );
	add_word( "LOOP", &forth_loop, 1 );

	add_word( "I", &forth_i, 0 );
	add_word( "J", &forth_j, 0 );
}

void word_to_flash(char *name_wrd, func *fnc){
	word_t wrd;
	copy(*(wrd.name), *name_wrd);
	wrd.funcptr = fnc;
//	flash_write_dict (,);
}

/*void read_word(){
	uintptr_t *help = (uintptr_t *)((flash_dict_now & ~0x3FF));
	char h[sizeof(word_t)*WORD_COUNT];
	int it = 0;

	while(1){
		while((*help != END_OF_PAGE) && (help <= ((uintptr_t *)((flash_dict_now & ~0x3FF))) + 1024 - 1 * 4)){
			++help;
			h[++it] = *help;
		}
		if (*help == END_OF_PAGE)
		{
			help = (uintptr_t *)*(help+1);
			--it;
		}
		else
		break;
	}
}*/

void add_word( char *name, func wordFunc, char flag )
{
	if( strlen( name ) >= 32 )
	{
		fault( "name too long " );
	}

	if( words.word_count >= WORD_COUNT )
	{
		fault( "too many words" );
	}	

	int word_pos = 0; //pos for new word
	int cmp_result;
	for( int i = 0; i < words.word_count; i++ )
	{
		cmp_result = cmp( name, words.word_array[i].name );

		if( cmp_result > 0 )
		{
			word_pos++;
		}
		else if( cmp_result == 0 ) // if word exist replase word
		{
			strcpy( words.word_array[i].name, name );
			words.word_array[i].funcptr = wordFunc;
			words.word_array[i].flag = flag | 0b10;
			return;
		}
		else
		{
			break;
		}
		
	} //calc pos for new word

	if( word_pos != words.word_count ) //chech: need move old words?
	{
		for( int i = words.word_count; i >= word_pos; i-- )
		{
			words.word_array[i + 1] = words.word_array[i];
		} //move old element
	}
	
	words.word_count++;

	strcpy( words.word_array[word_pos].name, name );
	words.word_array[word_pos].flag = flag;
	words.word_array[word_pos].funcptr = wordFunc; //paste new word
}

func get_word( char *name, status_t stat )
{
	int cmp_result;
	for( int i = 0; i <= words.word_count; i++ )

	{
		cmp_result = cmp( name, words.word_array[i].name );

		if( cmp_result == 0 )
		{
			if( words.word_array[i].flag & asm_run_only && stat == COMPILE )
				return words.word_array[i].funcptr;
			else if ( !(words.word_array[i].flag & asm_run_only) && stat == RUN )
			{
				return words.word_array[i].funcptr;
			}
			else
			{
				return NULL;
			}
		}
		else if( cmp_result == -1 )
		{
			return 0;
		}
	}	
		
	return 0;
}

/*func get_word( char *name)
{
	int cmp_result, i, a, b;
	i = words.word_count / 2;
	a = 0;
	b = words.word_count - 1;
	while( words.word_array[i].name != name)
	{
		cmp_result = cmp( name, words.word_array[i].name );

		if( cmp_result == 0 )
		{
			b = i;
			i = (a+b)/2;
		}
		else
		{
			a = i;
			i = (a+b)/2;
		}
	}

	return words.word_array[i].funcptr;
}*/

void rm_word( char *name )
{
	int cmp_result;
	int word_pos = -1;

	for( int i = 0; i < words.word_count; i++ )
	{
		cmp_result = cmp( name, words.word_array[i].name );

		if( cmp_result == 0 )
		{
			word_pos = i;	
			break;
		}
		else if( cmp_result == -1 )
		{
			return;
		}
	}

	if( word_pos == -1 )
	{
		return;
	}

	words.word_count--;

	if( word_pos == words.word_count )
	{
		return;
	}

	for( int i = word_pos; i < words.word_count; i++ )
	{
		words.word_array[i] = words.word_array[i+1];
	}
}
