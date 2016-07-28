#include "user_interface.h"
#include "stack.h"
#include "words.h"
#include "asm_compiler.h"
#include "kernel.h"
#include "flash.h"

/* run-time
 * 10 1
 * 10 2 - sys stack
 * DO - move values from user_stack to sys_stack
 * LOOP - remove values from sys_stack
 * I - copy value from sys_stack to user_stack
 * J - for second loop
 */

/* compile-time
 * token : conditional branch back, offset
 */

int main()
{
	init_UART();

	for( uint64_t i = 0; i < 250000; i++ ){;}//delay

	init_words();
	//flash_mass_erase();
	//flash_page_erase(0x20000 + 1024);
	initialize_code();

	UART_print( "HELLO PUTTY (or screen)\r\n" );

	uint16_t test_foo[64];

	char *ver = "  v0.5    by ws17\r\n";

	UART_print( ver );

	dispatch();

	return 0;
}
