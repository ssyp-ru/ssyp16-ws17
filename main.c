#include "user_interface.h"
#include "stack.h"
#include "words.h"
<<<<<<< HEAD
=======
#include "pwm.h"
>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc

int main ()
{
	init_UART();

<<<<<<< HEAD
	for( uint64_t i = 0; i < 500000; i++ ){;}//delay

	init_UART();
=======
	for( uint64_t i = 0; i < 250000; i++ ){;}//delay

>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
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
<<<<<<< HEAD
				push( dasha_atoi( token ) );
			}
		}
=======
				//push( dasha_atoi( token ) );
				push( string_to_int( token ) );
			}
		}

		drop_tokenise();
>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc
	}

	return 0;
}
