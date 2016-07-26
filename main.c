#include "user_interface.h"
#include "stack.h"
#include "words.h"
#include "asm_compiler.h"
#include "kernel.h"

int main ()
	{
	init_UART();

	for( uint64_t i = 0; i < 250000; i++ ){;}//delay

	init_words();

	UART_print( "HELLO PUTTY (or screen)\r\n" );

	char input[128];
	char token[32];

	func foo;

	uint16_t test_foo[64];

	char *ver = "  v0.1    by ws17\r\n";

	test_foo[0] = emit_push(8);
	test_foo[1] = emit_ldr_short( 12, 0 );
	test_foo[2] = emit_ldr_short( 4, 2 );
	test_foo[3] = emit_blx(2);
	test_foo[4] = emit_pop(8);
	test_foo[5] = 0x0;
	test_foo[6] = (UART_print)+1;
	test_foo[7] = 0x0;
	test_foo[9] = ( (uint32_t)ver ) >> 16;
	test_foo[8] = (uint16_t)ver;
	//test_foo[10] = 0x0;

	/*uint32_t ldr = emit_ldr_long( 10, 1 );

	test_foo[0] = emit_push(8);
	test_foo[1] = ldr;
	test_foo[2] = ldr >> 16;
	test_foo[3] = emit_cmp( 1, 1 );
	test_foo[4] = emit_beq( 0 );//0xD000;
	test_foo[5] = 2;
	test_foo[6] = emit_pop(8);
	test_foo[6] = emit_blx(1);
	test_foo[7] = 44;
	test_foo[8] = 0;
	test_foo[9] = 1;
	test_foo[10] = 1;*/

	func test_foo_c = (func)(((uintptr_t)test_foo)+1);

	test_foo_c();

	dispatch();

	return 0;
}
