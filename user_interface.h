#ifndef __USER_INTERFASE__
#define __USER_INTERFASE__

	void init_UART();

	void get_user_input( char *buffer );

	void UART_print( char *line );
	void UART_putc( char c );
	void UART_next_line();

	char UART_getc();

#endif
