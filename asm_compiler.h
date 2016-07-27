#include <stdlib.h>
#include <stdio.h>

#ifndef __ASM_COMPILER__
#define __ASM_COMPILER__

	uint32_t emit_bl( uint32_t start, uint32_t offset );
	uint32_t emit_ldr_long( uint16_t offset, uint8_t registr, uint8_t add );

	uint16_t emit_ldr_short( int offset, int registr );
	uint16_t emit_push( int registr );
	uint16_t emit_pop( int registr );
	uint16_t emit_mov( int sourse, int target );
	uint16_t emit_blx( int regist );

#endif
