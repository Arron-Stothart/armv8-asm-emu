movz x8, #1
movz x9, #1

request:
ldr x0, #0x82000
movz x1, #0x0
str #32, [x0, x1]
add x1, x1, #4
str #0, [x0, x1]
add x1, x1, #4
str #0x00038041, [x0, x1]
add x1, x1, #4
str #8, [x0, x1]
add x1, x1, #4
str #0x0, [x0, x1]
add x1, x1, #4
str #130, [x0, x1]
add x1, x1, #4
str x8, [x0, x1]
add x1, x1, #4
str #0x0, [x0, x1]

write_to_reg:
ldr x3, #0x82000
ldr x4, #8
add x3, x3, x4
ldr x5, #0x3f00b8a0
str x3, [x5]

wait:
movz x2, 100000
wait_l:
sub x2, x2, #1
cmp x2, #0x0
b.ne wait_l

read_check:
ldr x3, #0x3f00b898
ldr x3, [x3]
movz x4 #0x4000, lsl 16
ands xzr, x4, x3
b.ne read_check

ldr x0, #0x3f000b880
ldr x0, [x0]

eor x8, x8, x9

b request