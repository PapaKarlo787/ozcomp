get_6_names: ;uses r8 - start from
pushai
	mov r3, [total_files_in_dir]
	sub r3, r8
	sub r3, 6
	add r8, r3
	push r8
	mov r0, [cur_dir]
	xor r1, r1
	mov r5, r1
	mov r10, 32
.main_lp:
	call get_poi_to_data
	cmp r2, 0
	je .total_end
	movb r4, [r2]
	cmp r4, r10
	jl .end
	cmp r8, 0
	jne .skip
	mov r3, current_table
	add r3, r5
	mov r13, 3
.lp:
	mov r6, [r2]
	mov [r3], r6
	add r2, 4
	add r3, 4
	loop .lp
	
	and r6, 0x10000000
	je .dir
	mov r6, 0x206a20
	jmp .ndir
.dir:
	mov r6, 0x206820
.ndir:
	sub r3, 1
	mov [r3], r6

	add r3, 4
	add r2, 8
	movw r6, [r2]
	shl r6, 16
	add r2, 6
	movw r7, [r2]
	add r6, r7
	mov [r3], r6

	add r5, 19
	jmp .end
.skip:
	sub r8, 1
.end:
	add r1, r10
	jmp .main_lp
.total_end:
popai
ret
