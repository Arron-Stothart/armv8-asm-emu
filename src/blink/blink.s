dw:
intdir 10000

request:
movz x1, #0x3F00B880
str #256, x1
add x1, x1, #4
str #0, x1
add x1, x1, #4
str #0x00038041, x1
add x1, x1, #4
str #8, x1
add x1, x1, #4
str #0x0, x1
add x1, x1, #4
str #130, x1
add x1, x1, #4
str #0x1, x1
add x1, x1, #4
str #0x0, x1
b request_end

await_response:
b read_response

read_response:
b read_response_end

loop:
b request
request_end:
b await_response
read_response_end:
b wait

wait:
ldr x2, dw
wait_l:
sub x2, x2, #1
cmp x2, #0x0
b.ne wait_l
b loop