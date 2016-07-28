#ifndef __FORTH_COMPILER__
#define __FORTH_COMPILER__

	void define();

	void forth_if();
	void forth_else();
	void forth_then();

	void forth_do();
	void forth_loop();

	void r_dummy_handler(char * word);
	void compile_handler(char * word);
	void c_dummy_handler(char * word);
	int literul_handler(char * word);

	void forth_recurse();

#endif
