//#include <string.h>
//#include <stdio.h>
#include "words.h"
#include "fault.h"
#include "forth_simple.h"
#include "flash.h"
#include "string.h"

word_mass_t words;

#define cmp cmp
#define strlen len
#define strcpy copy
//int cmp( char *leftWord, char *rightWord );

void init_words()
{
	add_word( "+", &forth_add );
	add_word( "-", &forth_sub );
	add_word( "/", &forth_div );
	add_word( "*", &forth_mul );
	add_word( "%", &forth_mod );
	add_word( "/%", &forth_divmod );

	add_word( "swap", &forth_swap );
	add_word( "dup", &forth_dup );
	add_word( "rot", &forth_rot );
	add_word( "drop", &forth_drop );

	add_word( "cswap", &forth_cswap );
	add_word( "cdup", &forth_cdup );
	add_word( "crot", &forth_crot );
	add_word( "cdrop", &forth_cdrop );

	add_word( "&&", &forth_and );
	add_word( "||", &forth_or );

	add_word( "<", &forth_low );
	add_word( "<=", &forth_lowe );

	add_word( ">", &forth_hight );
	add_word( ">=", &forth_highte );

	add_word( "&", &forth_band );
	add_word( "|", &forth_bor );

	add_word( ".", &forth_print );
	add_word( ".S", &forth_print_all );

	add_word( "setmem", &forth_setmem );
	add_word( "getmem", &forth_getmem );
}

void word_to_flash(char *name_wrd, func *fnc){
	word_t wrd;
	copy(*(wrd.name), *name_wrd);
	wrd.funcptr = fnc;
	flash_write_dict (,);  	
}

void read_word(){
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
}

void add_word( char *name, func wordFunc )
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
	words.word_array[word_pos].funcptr = wordFunc; //paste new word
}

/*func get_word( char *name)
{
	int cmp_result;
	for( int i = 0; i <= words.word_count; i++ )

	{
		cmp_result = cmp( name, words.word_array[i].name );

		if( cmp_result == 0 )
		{
			return words.word_array[i].funcptr;
		}
		else if( cmp_result == -1 )
		{
			return 0;
		}
	}	
		
	return 0;
}*/

func get_word( char *name)
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
