get_poi_to_data:
; r0 - current start cluster
; r1 - position in file
; r2 - pointer to needed byte in memory
mov r2, [data_area]
push r4
push r3
push r1
push r0
mov r3, [first_fat]
.lp:
shl r0, 2
add r3, r0
mov r3, [r3]
cmp r3, 0xfffffff7
jge .ext
mov r0, r3
mov r4, [cluster_size]
sub r1, r4
add r2, r4
jmp .lp
.ext:
add r2, r1
pop r0
pop r1
pop r3
pop r4
ret
