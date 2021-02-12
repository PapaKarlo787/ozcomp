mov r0, 28
mov r1, 84
mov r2, 0
mov r3, 0
mov r4, 25
mov r5, 0
a:
delay 100
cls
bmp r1, 32, cact
bmp 3, r0, dino
draw
scur r4, r5
print score
iprint r3
sub r1, 3
sub r0, r2
cmp r1, -12
je reload
cont1:
cmp r0, 28
je stop
cont2:
cmp r0, 4
je down
cont3:
cmp r1, 30
je up
jmp a
reload:
mov r1, 84
add r3, 1
jmp cont1
stop:
mov r2, 0
jmp cont2
down:
mov r2, -2
jmp cont3
up:
mov r2, 3
jmp a
dino:
dd 20,3
dd 192,0,0,0,0,0,0,128,192,254,255,253,255,255,127,95,95,95,31,30
dd 15,31,62,124,252,254,255,255,255,255,255,127,63,31,2,6,0,0,0,0
dd 0,0,0,0,0,15,11,1,0,1,15,8,0,0,0,0,0,0,0,0
cact:
dd 12,2
dd 224,240,224,0,254,255,255,254,112,124,62,28
dd 1,3,7,7,255,255,255,255,0,0,0,0
score:
dd "Score: ",0
