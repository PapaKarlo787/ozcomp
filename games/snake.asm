cls
mov r13, 100
mov r0, 2
mov r1, 40
mov r2, 0
play sound
start:
cmp r13, 0
je left
cmp r13, 1
je up
cmp r13, 2
je right
cmp r13, 3
je down
jmp step

left:
cmp r0, 2
je step
jmp store_dir
up:
cmp r0, 3
je step
jmp store_dir
right:
cmp r0, 0
je step
jmp store_dir
down:
cmp r0, 1
je step

store_dir:
mov r0, r13

step:
mov r3, [heap+r1]
add r1, 4
mov r4, [heap+r1]
add r1, 4
cmp r0, 0
je draw_left
cmp r0, 1
je draw_up
cmp r0, 2
je draw_right

add r4, 2
jmp redraw
draw_left:
sub r3, 2
jmp redraw
draw_up:
sub r4, 2
jmp redraw
draw_right:
add r3, 2

redraw:
add r3, 84
mod r3, 84
add r4, 48
mod r4, 48
mov [heap+r1], r3
add r1, 4
mov [heap+r1], r4
sub r1, 4
rect r3, r4, 1, 1
scol 0
mov r3, [heap+r2]
add r2, 4
mov r4, [heap+r2]
add r2, 4
rect r3, r4, 1, 1
scol 1
mod r1, 8064
mod r2, 8064
rnd
mod r13, 10
delay 300
jmp start

heap:
times dd 0 40
dd 4
times dd 0 8044
sound:
dd 2, 1700, 1000, 900, 1000
