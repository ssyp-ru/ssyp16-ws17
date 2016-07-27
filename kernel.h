#ifndef __KERNEL__
#define __KERNEL__

	enum status
	{
		RUN	= 0,
		COMPILE	= 1,
		R_DUMMY = 2,
		C_DUMMY = 3,
		LITERAL = 4
	};

	extern enum status state;

	void dispatch();
	void run_handler(char * word);
	void r_dummy_handler(char * word);
	void compile_handler(char * word);
	void c_dummy_handler(char * word);
	void literul_handler(char * word);

	void compile_start();
	void compile_end();

	void forth_if();
	void forth_else();
	void forth_then();

#endif
