#include <stdint.h>
#include <string.h>
#include "flash.h"
#include "fault.h"

const uintptr_t
	BASE_FLASH_REGISTER = 0x400FD000, OFFSET_FMA = 0x000,
	OFFSET_FMD = 0x004,
	OFFSET_FMC = 0x008,
	BASE_SYSCTL_REGISTER = 0x400FE000, OFFSET_BOOTCFG = 0x1D0,
	START_FLASH=0x10240, END_FLASH=0x50000,
	END_OF_PAGE = 0xFFFFF;

uintptr_t flash_now = 0x10240;

void flash_write32(uint32_t num, uintptr_t ptr){
	*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMD)) = num;
	*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMA)) = ptr;
	if (!(*((uint32_t *)BASE_SYSCTL_REGISTER + OFFSET_BOOTCFG) & 0b10000))
		*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMC)) |= ((0xA442 << 16) | 0b1); // wrkey now = 0xA442 (magic constant) and write = 1
	else
		*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMC)) |= ((0x71D5 << 16) | 0b1);  // wrkey now = 0x71D5 (another magic constant) and write = 1
	while (*((uint32_t *)(BASE_FLASH_REGISTER + OFFSET_FMC)) & 0b1);
}

int flash_have_memory(size_t NumBite){
	if (NumBite <=  (flash_now & ~0x1FF) + 1024 - flash_now - 2 )
		return 1;
	else
		return 0;

}

uintptr_t flash_alloc(){
	int i = 0;
	while ((*((uintptr_t *)START_FLASH+i*1024) != 0xFFFFFFFFFF) || (START_FLASH+i*1024) == END_FLASH){
		++i;
	}
	if ((START_FLASH+i*1024) == END_FLASH) fault("there is no memory in flash");
	return START_FLASH+i*1024;
}

uintptr_t where_write(size_t NumBite){
	uintptr_t result, fn;
	if (flash_have_memory(NumBite))
		return flash_now;
	else{
		flash_write32(END_OF_PAGE, flash_now);
		fn =flash_now;
		flash_write32(result = flash_alloc(), fn);
	}
	return result;
}

void flash_write(uint32_t *num, size_t lengInt, uintptr_t ptr){
	if ((ptr & 0b11) != 0)
			fault("invalid ptr");
	for (size_t i = 0; i<lengInt; ++i)
		flash_write32(num[i], ptr + i * 4);
}

void flash_page_erase(uintptr_t ptr){
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
