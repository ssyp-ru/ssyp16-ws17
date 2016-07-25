#include <stdio.h>
#include <stdlib.h>

<<<<<<< HEAD
char *tokenise(char * string, char *result)
{
	static char * strCut;
	static char * str;

	if (str != string)
	{
		strCut =string;
		str=string; 
	}
=======
//static char * str;
int tokenise_reload = 1;

char *tokenise(char * string, char *result)
{
	static char * strCut;

	//if (str != string)
	//{
	//	strCut =string;
	//	str=string;
	//}

	if( tokenise_reload )
	{
		strCut = string;
		tokenise_reload = 0;
	}

>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
	if (*strCut == '\0')
		return NULL;
	while (*strCut == ' ' || *strCut == '\t')
		strCut++;
	// We determine the length of the word submitted
	size_t wordlen = 0;
	// ...

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

<<<<<<< HEAD
=======
void drop_tokenise()
{
	tokenise_reload = 1;
}

>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
int dasha_atoi(char * i)
{
	
	//'0' == 49
	//"123123" -> 123123
	//"123asd" -> 123
	//"asd" -> 0
	//9+'0' == '9'
<<<<<<< HEAD

=======
	for( int j = 0; i[j] != 0; j++ )
	{
		if( i[j] < '0' || i[j] > '9' )
		{
			UART_print( ">>> " );
			UART_print( i );
			UART_print( " <<<\r\n" );
			fault( "invaild command" );
		}
	}
>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
	
	int len,m = 1,num = 0;
	char * strCut = i;
	if (*strCut == '-')
	{
		m = -1;
		strCut++;
	}
<<<<<<< HEAD
=======

>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
	for (len = 0;(*strCut <= '9') && (*strCut >= '0');len++)
	{
		num*= 10;
		num += (*strCut - '0');
		strCut++;
	}
	num *= m;
	return num;
}

void Dasha_itoa(int i, char *str)
{
<<<<<<< HEAD
=======
	if( i == 0 )
	{
		str[0] = '0';
		str[1] = 0;
		return;
	}

>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
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
		///printf( "1: %d\n", e );
		str[w] = '0' + e;
		t *= 10;
	}
}

<<<<<<< HEAD
=======
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

// no
//void copy (char *f, char *s){	//Копирование строки в определённое место
//	for(int c = 0; c > len(s); c++){
//		f[c] = s[c];
//	}
//}

// no
/*char * dup (char *f){	//Копирование строки в неопределённое место
	char *s = (char *)malloc(len(f));
	for(int  c = 0; c < len(f); c ++){
		s[c] = f[c];
	}
	return s;
}*/


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
	switch ((int)ch)
	{
		case 'a':
			return 10;
		case 'b':
			return 11;
		case 'c':
			return 12;
		case 'd':
			return 13;
		case 'e':
			return 14;
		case 'f':
			return 15;
		case 'A':
			return 10;
		case 'B':
			return 11;
		case 'C':
			return 12;
		case 'D':
			return 13;
		case 'E':
			return 14;
		case 'F':
			return 15;
		default:
			return ((int)ch - 48);
	}
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

	for( int i = end_pos; text[i] != 0; i++ )
	{
		value*= mult;
		value+= int_from_char(text[i]);
	}

	return value * char_mult;
}

>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
