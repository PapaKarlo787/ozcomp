#ifdef OZEMU
void (*comms[256]) (void) = {
#else
void (*const comms[256]) (void) PROGMEM = {
#endif
	add_rr,
	add_rc,
	sub_rr,
	sub_rc,
	mul_rr,
	mul_rc,
	div_rr,
	div_rc,
	mov_rm,
	mov_mr,
	mov_rr,
	mov_rc,
	lea,
	rcond,
	and_rr,
	and_rc,
	or_rr,
	or_rc,
	xor_rr,
	xor_rc,
	cmp_rr,
	cmp_rc,
	jmp,
	jmp_c,
	lp,
	push,
	pop,
	mod_rr,
	mod_rc,
	print_num,
	print_line_c,
	del_r,
	del_c,
	snd_r,
	snd_c,
	to_int,
	get_key,
	set_cursor_r,
	set_cursor,
	draw_screen,
	call_c,
	ret,
	rnd,
	print_int,
	nop,
	nop,
	nop,
	nop,
	pow_rr,
	pow_rc,
	point_rr,
	point_cc,
	circle_r,
	circle_c,
	circle_rc,
	line_r,
	line_c,
	line_rc,
	clear_screen,
	rect_r,
	rect_c,
	rect_rc,
	bmp_rc,
	bmp_cr,
	bmp_rr,
	bmp_cc,
	scond,
	f_add_rr,
	f_add_rc,
	f_sub_rr,
	f_sub_rc,
	f_mul_rr,
	f_mul_rc,
	f_div_rr,
	f_div_rc,
	f_pow_rr,
	f_pow_rc,
	f_cmp_rr,
	f_cmp_rc,
	shr_rr,
	shr_rc,
	shl_rr,
	shl_rc,
	fmov_rm,
	fmov_mr,
	fmov_rr,
	fmov_rc,
	kread,
	kwrite,
	fpush,
	fpop,
	play,
	nplay,
	imovf,
	fmovi,
	jmp_r,
	jmp_c_r,
	lp_r,
	call_r,
	nop,
	nop,
	nop,
	nop,
	print_line_r,
	push_c,
	pushai,
	popai,
	pushaf,
	popaf,
	rpix_r,
	rpix_c,
	mzer_r,
	mzer_c,
	test,
	test_f,
	set_time,
	get_time,
	mls,
	mcs,
	rgb_r,
	rgb_c,
	nop,
	nop,
	nop,
	test_rr,
	test_rc,
	load_reg,
	store_reg,
	mcp,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop,
	nop
};
