get_6_names:
pushai
	mov r0, [cur_dir]
	xor r1, r1
	mov r5, r1
	mov r10, 32
.main_lp:
	call get_poi_to_data
	movb r4, [r2+0xb]
	cmp r4, r10
	jl .end
	mov r3, current_table
	add r3, r5
	mov r13, 3
.lp:
	mov r6, [r2]
	mov [r3], r6
	add r2, 4
	add r3, 4
	loop .lp

	add r2, 8
	movw r6, [r2]
	shl r6, 16
	add r2, 6
	mov r7, [r2]
	add r6, r7
	mov [r3], r6
	sub r3, 1
	xor r6, r6
	movb [r3], r6
	add r5, 16
	cmp r5, 96
	je .total_end
.end:
	add r1, r10
	jmp .main_lp
.total_end:
popai
ret
