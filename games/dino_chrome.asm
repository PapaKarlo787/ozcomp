cls
mov r0, 28
mov r1, 84
mov r2, 0
mov r3, r2
scur 25, 0
lprint score
bmp 3, r0, dino
play sound
aa:
scol 0
call draw_map
scol 1
sub r1, 3
sub r0, r2
call draw_map
ji lp
scur 67, 0
print r3

cmp r1, -12
je reload
cont1:
cmp r0, 28
je stop
cont2:
cmp r0, 4
je down
cont3:
gkey
cmp r15, 57
je up
jmp aa
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
cmp r2, -2
je aa
mov r2, 3
jmp aa
draw_map:
cmp r2, 0
je draw_cact
bmp 3, r0, dino
draw_cact:
delay 50
bmp r1, 32, cact
ret
lp:
nplay
delay 1000
ret
dino:
db 20,3
db 192,0,0,0,0,0,0,128,192,254,255,253,255,255,127,95,95,95,31,30
db 15,31,62,124,252,254,255,255,255,255,255,127,63,31,2,6,0,0,0,0
db 0,0,0,0,0,15,11,1,0,1,15,8,0,0,0,0,0,0,0,0
cact:
db 12,2
db 224,240,224,0,254,255,255,254,112,124,62,28
db 1,3,7,7,255,255,255,255,0,0,0,0
score:
db "Score: ",0
sound:
dd 39
dw 392, 350, 392, 350, 392, 350, 311, 250, 466, 100, 392, 350, 311, 250
dw 466, 100, 392, 700, 587, 350, 587, 350, 587, 350, 622, 250, 466, 100
dw 369, 350, 311, 250, 466, 100, 392, 700, 784, 350, 392, 250, 392, 100
dw 784, 350, 739, 250, 698, 100, 659, 100, 622, 100, 659, 450, 415, 150
dw 554, 350, 523, 250, 493, 100, 466, 100, 440, 100, 466, 450, 311, 150
dw 369, 350, 311, 250, 466, 100, 392, 750
