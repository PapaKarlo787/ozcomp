movw r0, [0xe]
shl r0, 9
add r0, 512
mov [first_fat], r0
movb r1, [0x10]
movw r2, [0x16]
shl r2, 9
mov [fat_size], r2
mul r1, r2
add r0, r1
mov [root_dir], r0



first_fat:
dd 0
root_dir:
dd 0
fat_size:
dd 0
