movw r0, [0xe]
shl r0, 9
add r0, 512
mov [first_fat], r0
mov r1, [0xd]
shl r1, 9
mov [cluster_size], r1
movb r1, [0x10] 	;number of fats
movw r2, [0x16] 	;sector per fat
shl r2, 9
mov [fat_size], r2
mul r1, r2
add r0, r1
mov [data_area], r0



first_fat:
dd 0
cluster_size:
dd 0
data_area:
dd 0
fat_size:
dd 0
cur_dir:
dd 2


get_poi_to_data:
; r0 - current start cluster
; r1 - position in file
mov r2, [data_area]
push r3
push r1
mov r3, [first_fat]
add r3, r0
mov r3, [r3]
pop r1
pop r3
