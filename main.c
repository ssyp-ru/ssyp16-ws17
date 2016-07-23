#include "user_interface.h"
#include "stack.h"
#include "words.h"
#include "pwm.h"

int main ()
{
	init_UART();

	for( uint64_t i = 0; i < 250000; i++ ){;}//delay

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
				//push( dasha_atoi( token ) );
				push( string_to_int( token ) );
			}
		}

		drop_tokenise();
	}

	return 0;
}
