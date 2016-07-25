#include "user_interface.h"
#include "stack.h"
#include "words.h"

int main ()
{
	init_UART();

	for( uint64_t i = 0; i < 500000; i++ ){;}//delay

	init_UART();
	init_words();

	UART_print( "HELLO PUTTY (or screen)\r\n" );

	char input[128];
	char token[32];

	func foo;

	while(1)
	{
		get_user_input( input );

		while( tokenise( input, token ) )
		{
			foo = get_word( token );
			if( foo != 0x0 )
			{
				foo();
			}
			else
			{
				push( dasha_atoi( token ) );
			}
		}
	}

	return 0;
}
