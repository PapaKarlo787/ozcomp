main:
	xor r0, r0 ; pointer to line
	mov r1, r0
	call get_total_files
	call get_6_names
	call print_names
	.lp
		gkey
		cmp r15, 1
		je .up
		cmp r15, 2
		je .down
		cmp r15, 3
		jne .lp


	jmp .lp
.up:

	jmp .lp
.down:

	jmp .lp


current_table:
times db 0 114
el_in_tab:
db 0
total_files_in_dir:
dd 0

include "get_menu_names.asm"


print_names:
	mov r0, current_table
	mov r13, [total_files_in_dir]
	cmp r13, 6
	jle .start
	mov r13, 6
.start:
	lprint r0
	add r0, 19
	loop .start
ret

get_total_files:
	mov r0, [cur_dir]
	xor r1, r1
	mov r3, r1
	mov r10, 32
.lp:
	call get_poi_to_data
	cmp r2, 0
	je .ext
	add r1, r10
	movb r4, [r2]
	cmp r4, r10
	jl .lp
	add r3, 1
	jmp .lp
.ext:
	mov [total_files_in_dir], r3
ret
