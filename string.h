#ifndef __STRINGS__S
#define __STRINGS__S

	#define charToInt dasha_atoi

	int charToInt( char *i );
	void Dasha_itoa( int i, char *str );
	char *tokenise( char *string, char *result );
<<<<<<< HEAD
=======
	void drop_tokenise();

	int len( char *f );
	void copy( char *f, char *s );
	int cmp( char *f, char *s );

	int string_to_int( char *text );
>>>>>>> 286ab3220af06a8712f93b7b43a127a9b8c41fbc

#endif
