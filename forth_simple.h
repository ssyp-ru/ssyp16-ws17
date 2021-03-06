#ifndef __FORTH_SIMPLE__
#define __FORTH_SIMPLE__

	void parentheses();
	void quote();
	void define();

	void forth_add();
	void forth_sub();
	void forth_div();
	void forth_mul();
	void forth_mod();
	void forth_divmod();

	void forth_swap();
	void forth_dup();
	void forth_rot();
	void forth_drop();
	void forth_over();

	void forth_cswap();
	void forth_cdup();
	void forth_crot();
	void forth_cdrop();

	void forth_and();
	void forth_or();
	void forth_low();
	void forth_hight();
	void forth_lowe();
	void forth_highte();
	void forth_band();
	void forth_bor();
	void forth_eq();

	void forth_print();
	void forth_print_all();

	void forth_setmem();
	void forth_getmem();

	void forth_i();
	void forth_j();

	void forth_data_to_ctrl();
	void forth_ctrl_to_data();

#endif
