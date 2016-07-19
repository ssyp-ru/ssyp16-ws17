#include <string.h>
#include <stdio.h>
#include "words.h"
#include "fault.h"

word_mass_t words;

#define cmp strcmp
//int cmp( char *leftWord, char *rightWord );

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

	if( word_pos != words.word_count-1 ) //chech: need move old words?
	{

		for( int i = word_pos + 1; i < words.word_count; i++ )
		{
			words.word_array[i + 1] = words.word_array[i];
		} //move old element
		words.word_count++;
	}

	strcpy( words.word_array[word_pos].name, name );
	words.word_array[word_pos].funcptr = wordFunc; //paste new word
}

func get_word( char *name)
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
