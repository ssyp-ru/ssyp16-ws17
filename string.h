#ifndef __STRINGS__S
#define __STRINGS__S

	#define charToInt dasha_atoi

	int charToInt( char *i );
	void Dasha_itoa( int i, char *str );
	char *tokenise( char *string, char *result );
	void drop_tokenise();

#endif
