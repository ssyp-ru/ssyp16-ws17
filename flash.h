#include <stdint.h>
#include <string.h>

extern uintptr_t flash_now;

void flash_write32(uint32_t num, uintptr_t ptr);

void flash_write(uint32_t *num, size_t lengInt, uintptr_t ptr);

void flash_page_erase(uintptr_t ptr);

void flash_mass_erase();

int flash_have_memory(size_t NumBite);
