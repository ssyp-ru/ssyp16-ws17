#ifndef __KERNEL__
#define __KERNEL__

	enum status
	{
		RUN	= 0,
		COMPILE	= 1
	};

	typedef enum status status_t;

	extern enum status state;
	extern char buffer[255], word[64];

	void dispatch();
	void run_handler(char * word);

	void c_dummy_handler(char * word);
	int r_dummy_handler(char * word);

	void compile_start();
	void compile_end();

#endif
