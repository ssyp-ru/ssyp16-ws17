#include <stdint.h>
#include <string.h>
#include "flash.h"
#include "fault.h"

const uintptr_t
	BASE_FLASH_REGISTER = 0x400FD000, OFFSET_FMA = 0x000,
	OFFSET_FMD = 0x004,
	OFFSET_FMC = 0x008,
	BASE_SYSCTL_REGISTER = 0x400FE000, OFFSET_BOOTCFG = 0x1D0,
	END_FLASH=0x50000,
	END_OF_PAGE = 0xFFFFF;

uintptr_t flash_dict_now = 0xB600;

uintptr_t flash_code_now = 0xB600 + 1024;

void flash_write32(uint32_t num, uintptr_t ptr){
	*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMD)) = num;
	*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMA)) = ptr;
	if (!(*((uint32_t *)BASE_SYSCTL_REGISTER + OFFSET_BOOTCFG) & 0b10000))
		*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMC)) |= ((0xA442 << 16) | 0b1); // wrkey now = 0xA442 (magic constant) and write = 1
	else
		*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMC)) |= ((0x71D5 << 16) | 0b1);  // wrkey now = 0x71D5 (another magic constant) and write = 1
	while (*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMC)) & 0b1);
}

int flash_have_memory_dict(size_t num_bite){
	if (num_bite * 4 <= (flash_dict_now & ~0x3FF) + 1024 - flash_dict_now - 2 )
		return 1;
	else
		return 0;
}

int flash_have_memory_code(size_t num_bite){
	if (num_bite * 4 <= (flash_code_now & ~0x3FF) + 1024 - flash_code_now - 2 )
		return 1;
	else
		return 0;
}

uintptr_t flash_alloc_dict(){
	if ((flash_dict_now & ~0x3FF) > (flash_code_now & ~0x3FF))
	return (flash_dict_now & ~0x3FF) + 1024;
	return (flash_code_now & ~0x3FF) + 1024;
}

uintptr_t flash_alloc_code(){
	if ((flash_dict_now & ~0x3FF) > (flash_code_now & ~0x3FF))
	return (flash_dict_now & ~0x3FF) + 1024;
	return (flash_code_now & ~0x3FF) + 1024;
}

uintptr_t where_write_dict(size_t num_bite){
	uintptr_t result, fn;
	if (flash_have_memory_dict(num_bite))
		return flash_dict_now;
	else{
		flash_write32(END_OF_PAGE, flash_dict_now);
		flash_dict_now +=1*4;
		fn =flash_dict_now;
		flash_write32(flash_dict_now = result = flash_alloc_dict(), fn);
		return result;
	}
}

void initialize_dict(){
	uintptr_t *help;
	while(1){
		help = (uintptr_t *)((flash_dict_now & ~0x3FF) + 1024 - 1 * 4);
		while((*help == 0xFFFFFFFF) && (help >= ((uintptr_t *)((flash_dict_now & ~0x3FF))))){
			help-=1;
		}
		if (*(help-1) == END_OF_PAGE)
				flash_dict_now = *help;
		else
		{
			flash_dict_now = (uintptr_t)(help+1);
			break;
		}
	}
}

void initialize_code(){
	uintptr_t *help;
	while(1){
		help = (uintptr_t *)((flash_code_now & ~0x3FF) + 1024 - 1 * 4);
		while((*help == 0xFFFFFFFF) && (help >= ((uintptr_t *)((flash_code_now & ~0x3FF))))){
			help-=1;
		}
		if (*(help-1) == END_OF_PAGE)
				flash_code_now = *help;
		else
		{
			flash_code_now = (uintptr_t)(help+1);
			break;
		}
	}
}

uintptr_t where_write_code(size_t num_bite){
	uintptr_t  fn;
	if (flash_have_memory_code(num_bite))
		return flash_code_now;
	else{
		flash_write32(END_OF_PAGE, flash_code_now);
		flash_code_now +=1*4;
		fn =flash_code_now;
		flash_write32(flash_code_now = flash_alloc_code(), fn);
		return flash_code_now;
	}
}

void flash_write(uint32_t *num, size_t leng_int, uintptr_t ptr){
	if ((ptr & 0b11) != 0)
			fault("invalid ptr");

	for (size_t i = 0; i<leng_int; ++i)
		flash_write32(num[i], ptr + i * 4);
}

void flash_write_dict(uint32_t *num, size_t leng_int){
	flash_write(num, leng_int, where_write_dict(leng_int));
	flash_dict_now += leng_int * 4;
}

void flash_write_code(uint32_t *num, size_t leng_int){
	flash_write(num, leng_int, where_write_code(leng_int));
	flash_code_now += leng_int * 4;
}

void flash_page_erase(uintptr_t ptr){
	if ((ptr & 0b11) != 0)
			fault("invalid ptr");

	*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMA)) = ptr;
	if (!(*((uint32_t *)BASE_SYSCTL_REGISTER + OFFSET_BOOTCFG) & 0b10000))
		*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMC)) |= ((0xA442 << 16) | 0b10); // wrkey now = 0xA442 (magic constant) and erase = 1
	else
		*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMC)) |= ((0x71D5 << 16) | 0b10); // wrkey now = 0x71D5 (another magic constant) and erase = 1

	while (*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMC)) & 0b10); //
}

void flash_mass_erase(){
	if (!(*((uint32_t *)BASE_SYSCTL_REGISTER + OFFSET_BOOTCFG) & 0b10000))
		*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMC)) |= ((0xA442 << 16) | 0b100); // wrkey now = 0xA442 (magic constant) and werase = 1
	else
		*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMC)) |= ((0x71D5 << 16) | 0b100); // wrkey now = 0x71D5 (another magic constant) and werase = 1

	while (*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMC)) & 0b100); //
}
