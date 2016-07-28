#ifndef __FORTH_COMPILER__
#define __FORTH_COMPILER__

	void define();

	void forth_if();
	void forth_else();
	void forth_then();

	void forth_do();
	void forth_loop();

	void compile_handler(char * word);
	int literul_handler(char * word);

	void forth_recurse();

	void define_compile_time();

#endif
