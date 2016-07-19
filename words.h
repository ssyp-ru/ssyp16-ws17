#include <stdlib.h>

#define WORD_COUNT 1024

typedef void (*func)(void);

typedef struct word
{
	char name[32];
	func funcptr;
} word_t;

typedef struct word_mass
{
	word_t word_array[WORD_COUNT];
	int word_count;
} word_mass_t;

extern word_mass_t worlds;

void init_words();

func get_word( char *name );
void add_word( char *name, func wordFunc );
void rm_word( char *name );
