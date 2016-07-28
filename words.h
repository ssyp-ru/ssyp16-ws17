#include <stdlib.h>
#include "kernel.h"

#ifndef __WORDS__
#define __WORDS__

	#define MAX_WORD_COUNT 256
	#define WORD_LENGTH 9
	typedef void (*func)(void);

	typedef struct word
	{
		char name[32];
		func funcptr;
		char flag;
	} word_t;

	typedef struct word_mass
	{
		word_t word_array[MAX_WORD_COUNT];
		int word_count;
	} word_mass_t;

	typedef enum word_flag_en
	{
		asm_run_only = 0b1,
		reset = 0b10
	} word_flag_t;

	extern word_mass_t words;

	void init_words();

	void word_to_flash(char *name_wrd, func *fnc);

	func get_word( char *name, status_t stat );
	char get_word_flag( char *word, word_flag_t flag );
	void add_word( char *name, func wordFunc, char flag );

	void rm_word( char *name );
	
	void initWords();

#endif
