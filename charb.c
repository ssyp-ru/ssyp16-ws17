#include <stdio.h>
#include <stdlib.h>

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
}

// no
//void copy (char *f, char *s){	//Копирование строки в определённое место
//	for(int c = 0; c > len(s); c++){
//		f[c] = s[c];
//	}
//}

// no
char * dup (char *f){	//Копирование строки в неопределённое место
	char *s = (char *)malloc(len(f)); 
	for(int  c = 0; c < len(f); c ++){ 		
		s[c] = f[c];
	}
	return s; 
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

/*int cmp(char *f, char *s){	//Сравнение строк
	while(*f != 0 && *s != 0){
		if(*f != *s) {
			if(*f > *s){
				return 1;
			} else {
				return -1;
			}
		}
	}
	return 0;
}*/

// NO!
/*char cat(char *f, char *s){	//Соединение строк
	f = len(f) + 1;
	for(int c = 0; c < len(s); c ++){
		*(f + c) = *(s + c);
	}
	return f;
}*/
