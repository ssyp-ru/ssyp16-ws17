#ifndef __STRINGS__S
#define __STRINGS__S

	#define charToInt dasha_atoi

	int charToInt( char *i );
	void Dasha_itoa( int i, char *str );
	char *tokenise( char *string, char *result );
	void drop_tokenise();

	int len( char *f );
	void copy( char *f, char *s );
	int cmp( char *f, char *s );

#endif
