#include <stdlib.h>
#include <stdio.h>
#include "kernel.h"
#include "flash.h"

#ifndef __WORDS__
#define __WORDS__

	#define MAX_WORD_COUNT 256
	#define WORD_LENGTH 9
	typedef void (*func)(void);

	typedef struct word
	{
		char name[32];
		func funcptr;
		uintptr_t flag;
	} word_t;

	typedef struct word_mass
	{
		word_t word_array[MAX_WORD_COUNT];
		int word_count;
	} word_mass_t;

	typedef enum word_flag_en
	{
		asm_run_only = 0b1,
		reset = 0b10,
		run_always = 0b100
	} word_flag_t;

	extern word_mass_t words;

	void init_words();

	void word_to_flash(char *name_wrd, func fnc, char *flag);
	void init_words_from_flash();
	void init_words_to_flash();
	void word_from_flash(uintptr_t addr_flash);

	func get_word( char *name, status_t stat );
	char get_word_flag( char *word, word_flag_t flag );
	void add_word( char *name, func wordFunc, char flag );
	void rm_word( char *name );
	
	void initWords();

#endif
