#include <stdio.h>
#include <stdlib.h>

char *tokenise(char * string, char *result)
{
	static char * strCut;
	static char * str;

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

