#include "user_interface.h"
#include "stack.h"
#include "words.h"

int main ()
	{

	init_UART();

	UART_print( "HELLO PUTTY (or screen)\r\n" );

	char input[128];
	char *token;

	int num;

	func foo;

	while(1)
	{
		get_user_input( input );

		while( tokenise( &(input[0]), token ) )
		{
			foo = get_word( token );
			if( foo != 0x0 )
			{
				foo();
			}
			else
			{
				num = dasha_atoi( token );
			}
		}
	}

	return 0;
}
