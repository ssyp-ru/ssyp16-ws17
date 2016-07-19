#include "words.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void foo()
{
	printf("foo n1\n");
}

void foo2()
{
	printf("foo n2\n");
}

int main()
{
	add_word( "one", &foo );
	(*(get_word( "one" )))();
	
	add_word( "two1", &foo2 );
	add_word( "one", &foo2 );
	add_word( "two3", &foo2 );

//	printf( "SIZE %d \n", words.word_count );
	
	assert( get_word("two1") != 0 );
	assert( get_word("one") != 0);
	assert( get_word("two3") != 0);
	
	rm_word( "two1" );
	
	rm_word( "two2" );

	assert( get_word("one") != 0);
	assert( get_word("two3") != 0);
	assert( get_word("two1") == 0);

	(get_word("two3"))();
	(get_word("one"))();
	return 0;
}
