print str
delay 1000
cls
line 0, 0, 83, 47
line 83, 47, 0, 0
line 0, 48, 84, 0
line 84, 0, 0, 48

a:
mov r0, r13
mod r0, 10
add r0, 5
rnd
mov r1, r13
mod r1, 84
rnd
mod r13, 48
circle r0, r1, r13
delay 200
jmp a
str:
dd "Hello, world!", 0


rnd
