#include <stdlib.h>
#include <stdio.h>
#include "fault.h"

void fault( char *error )
{
	printf( "Program crash with error: %s \n", error );
	exit( 1 );
}
