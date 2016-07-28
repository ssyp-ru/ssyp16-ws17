//#include <string.h>
//#include <stdio.h>
#include "words.h"
#include "fault.h"
#include "forth_simple.h"
#include "kernel.h"
#include "flash.h"
#include "string.h"
#include "forth_compiler.h"

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

	add_word( "over", &forth_over, 0 );

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
	add_word( "(", &parentheses, run_always );
	add_word( ".\"", &quote, 1 );

	add_word( ":", &define, 0 );
	add_word( "::", &define_compile_time, 0 );
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

	add_word( "recurse", &forth_recurse, 1 );

	add_word( "R>", &forth_ctrl_to_data, 0 );
	add_word( "R<", &forth_data_to_ctrl, 0 );
}

void word_to_flash(char *name_wrd, func *fnc){
	word_t wrd;
	for(int c = 0; c < 32; c++){
		wrd.name[c] = 0;
	}
	copy(wrd.name, name_wrd);
	wrd.funcptr = fnc;
	flash_write_dict (&wrd, (sizeof (word_t)) / 4);
}

void init_words_to_flash()
{
	word_to_flash( "+", &forth_add );
	word_to_flash( "-", &forth_sub );
	word_to_flash( "/", &forth_div );
	word_to_flash( "*", &forth_mul );
	word_to_flash( "%", &forth_mod );
	word_to_flash( "/%", &forth_divmod );

	word_to_flash( "swap", &forth_swap );
	word_to_flash( "dup", &forth_dup );
	word_to_flash( "rot", &forth_rot );
	word_to_flash( "drop", &forth_drop );

	word_to_flash( "cswap", &forth_cswap );
	word_to_flash( "cdup", &forth_cdup );
	word_to_flash( "crot", &forth_crot );
	word_to_flash( "cdrop", &forth_cdrop );

	word_to_flash( "&&", &forth_and );
	word_to_flash( "||", &forth_or );

	word_to_flash( "<", &forth_low );
	word_to_flash( "<=", &forth_lowe );

	word_to_flash( ">", &forth_hight );
	word_to_flash( ">=", &forth_highte );

	word_to_flash( "&", &forth_band );
	word_to_flash( "|", &forth_bor );

	word_to_flash( ".", &forth_print );
	word_to_flash( "(", &parentheses);
	word_to_flash( "\"", &quote);

	word_to_flash( ":", &define);
	word_to_flash( ";", &compile_end);
	word_to_flash( "|", &forth_bor );

	word_to_flash( ".S", &forth_print_all );

	word_to_flash( "@", &forth_setmem );
	word_to_flash( "!", &forth_getmem );
}

void word_from_flash(uintptr_t addr_in_flash){
	char name_wrd[32];
	func func_wrd;
	int *word_in_flash = (char *)addr_in_flash;
	copy(name_wrd[32], *word_in_flash);
	word_in_flash += 32;
	func_wrd = *word_in_flash;
	add_word(name_wrd, func_wrd, 0);
}

void add_word( char *name, func wordFunc, char flag )
{
	char buffer[32];
	copy( buffer, name );
	to_lower( buffer );
	if( len( buffer ) >= 32 )
	{
		fault( "name too long " );
	}

	if( words.word_count >= MAX_WORD_COUNT )
	{
		fault( "too many words" );
	}

	int word_pos = 0; //pos for new word
	int cmp_result;
	for( int i = 0; i < words.word_count; i++ )
	{
		cmp_result = cmp( buffer, words.word_array[i].name );

		if( cmp_result > 0 )
		{
			word_pos++;
		}
		else if( cmp_result == 0 ) // if word exist replase word
		{
			copy( words.word_array[i].name, buffer );
			words.word_array[i].funcptr = wordFunc;
			words.word_array[i].flag = flag | reset;
			return;
		}
		else
		{
			break;
		}
		
	} //calc pos for new word

	if( word_pos != words.word_count ) //check: need move old words?
	{
		for( int i = words.word_count; i >= word_pos; i-- )
		{
			words.word_array[i + 1] = words.word_array[i];
		} //move old element
	}
	
	words.word_count++;

	copy( words.word_array[word_pos].name, buffer );
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
			if( words.word_array[i].flag & run_always )
			{
				return words.word_array[i].funcptr;
			}
			else if( words.word_array[i].flag & asm_run_only && stat == COMPILE )
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
