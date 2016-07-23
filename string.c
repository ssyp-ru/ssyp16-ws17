#include <stdio.h>
#include <stdlib.h>

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

void drop_tokenise()
{
	tokenise_reload = 1;
}

int dasha_atoi(char * i)
{
	
	//'0' == 49
	//"123123" -> 123123
	//"123asd" -> 123
	//"asd" -> 0
	//9+'0' == '9'
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
	
	int len,m = 1,num = 0;
	char * strCut = i;
	if (*strCut == '-')
	{
		m = -1;
		strCut++;
	}

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
		///printf( "1: %d\n", e );
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
