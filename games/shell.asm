add r0, 10
mov r14, 5
x:
mul r0, 10
scur 0, 0
iprint r0
delay 1000
loop x
ee:
delay 1000
jmp ee
delay 1000
e:
dd "EhehehE"
