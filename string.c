#include <stdio.h>
#include <stdlib.h>

static char * str;

char *tokenise(char * string, char *result)
{
	static char * strCut;

	if (str != string)
	{
		strCut =string;
		str=string; 
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
	str = 0x0;
}

int dasha_atoi(char * i)
{
	
	//'0' == 49
	//"123123" -> 123123
	//"123asd" -> 123
	//"asd" -> 0
	//9+'0' == '9'

	
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

