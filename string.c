#include <stdio.h>
#include <stdlib.h>

char * str;
char * strCut;

char *tokenise(char * string, char *result)
{

	if (str != string)
	{
		strCut =string;
		str=string;
	}

	if ( *strCut == '\0' || string == NULL )
		return NULL;
	while (*strCut == ' ' || *strCut == '\t')
		strCut++;
	// We determine the length of the word submitted
	size_t wordlen = 0;
	// ...

	if ( *strCut == 0 )
			return NULL;

	while  ((*strCut != ' ' ) && (*strCut != '\0') && (*strCut !='\t'))
	{
		wordlen++;
		strCut++;
	}
	char * caret = strCut-1;

	for (int i = wordlen-1; i >= 0; i--)
	{
		result[i] = *(caret--);
	}
	result[wordlen] = 0;	

	return result;
}

void Dasha_itoa(int i, char *str)
{
	if( i == 0 )
	{
		str[0] = '0';
		str[1] = 0;
		return;
	}

	int len = -1, tmp = 1, t = 1, i2 = i;
	if ( i < 0 )
	{
		str[0] = '-';
		len ++;
		i2 *= -1;

	}
	while (tmp != 0)
	{
		tmp = i2 / t;
		len ++;
		t *= 10;
	}
	str[len] = 0;
	len--;
	t = 10;
	int end = i > 0 ? 0 : 1;
	for (int w = len; w >= end; w--)
	{
		int e = (i2 % t) / (t / 10);
		str[w] = '0' + e;
		t *= 10;
	}
}

int len (char *f){	//Нахождение длины строки
	int lenstr = 0;
	while(*f != 0) {
		lenstr++;
		f++;
	}
	return lenstr;
}

void copy( char *f, char *s )
{
	int i = 0;
	while( s[i] != 0 )
	{
		f[i] = s[i];
		i++;
	}
	f[i] = s[i];
}

int cmp( char *f, char *s )
{
	while( 1 )
	{
		if( *f != *s )
		{
			return (*f) - (*s);
		}
		if( *f == 0 || *s == 0 )
		{
			return (*f) - (*s);
		}
		f++;
		s++;
	}
	return 0;
}

int int_from_char( char ch )
{
	if( ch >= '0' && ch <= '9' )
	{
		return (ch - '0');
	}
	else if( ch >= 'a' && ch <= 'f' )
	{
		return (ch - 'a' + 10);
	}

	return (ch - 'A' + 10);
}

int string_to_int( char *text ) //by dima
{
	int mult = 10;
	int value = 0;
	int end_pos = 0;
	int char_mult = 1;

	if( text[0] == '-' )
	{
		char_mult = -1;
		end_pos++;
	}

	if( text[end_pos] == '0' )
	{
		if( text[end_pos+1] == 'x' )
		{
			mult = 16;
			end_pos+= 2;
		}
		else if( text[end_pos+1] == 'b' )
		{
			mult = 2;
			end_pos+= 2;
		}
		else
		{
			mult = 8;
			end_pos+= 1;
		}
	}

	int num;

	for( int i = end_pos; text[i] != 0; i++ )
	{
		value*= mult;
		num = int_from_char(text[i]);

		if( num < 0 || num > 16 )
		{
			UART_print( ">>> " );
			UART_print( text );
			UART_print( " <<<\r\n" );

			fault( "unknown command" );
		}

		value+= num;
	}

	return value * char_mult;
}

