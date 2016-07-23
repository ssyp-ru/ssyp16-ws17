#include "flash.h"
#include <stdint.h>
#include <stdio.h>

const intptr_t free_flash_memory = 0x10240;

uint32_t masss[1000];

int main(){
	for(int i=0; i<1000; ++i) masss[i]=1;
	masss[0]=(0x11111111);
	masss[1]=(0x01111111);
	masss[2]=(0x10111111);
	masss[3]=(0x11011111);
	masss[4]=(0x11101111);
	masss[5]=(0x11111011);
	masss[6]=(0x11111101);
	masss[7]=(0x11111110);
	masss[8]=(0x00111111);
	flash_page_erase(where_write_dict(0));
	flash_write_dict(masss, 9);
	masss[0]=(0x22222222);
	masss[1]=(0x02222222);
	masss[2]=(0x20222222);
	masss[3]=(0x22022222);
	masss[4]=(0x22222222);
	masss[5]=(0x22222022);
	masss[6]=(0x22222202);
	masss[7]=(0x22222220);
	masss[8]=(0x00222222);
	flash_write_dict(masss, 9);
	flash_write_code(masss, 9);
	return 0;
}
