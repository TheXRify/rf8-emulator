lia 2
lib 3
lic 1
sta 0x0000
stb 0x0001
sta 0x0002

ldl 0x0001
cmp l, c
jeq 51
ldh 0x0000
ldl 0x0002
mul
ldl 0x0001
dec l
stl 0x0001
sth 0x0000
jmp 18

hlt