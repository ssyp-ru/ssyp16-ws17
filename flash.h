#include <stdint.h>
#include <string.h>
#include <stdio.h>

extern uintptr_t flash_dict_now;

extern uintptr_t flash_code_now;

extern const uintptr_t END_OF_PAGE;

void flash_page_erase(uintptr_t ptr);

void flash_mass_erase();

uintptr_t flash_write_code(uint32_t *num, size_t leng_int);

void flash_write_dict(uint32_t *num, size_t leng_int);

void initialize_dict();

void initialize_code();

uintptr_t where_write_code(size_t num_bite);

uintptr_t where_write_dict(size_t num_bite);
