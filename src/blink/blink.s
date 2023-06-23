dw:
intdir 10000


request:
movz x1, #0x3F00B880
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


COMMENTS:
0x3f000000 + 0xb880 = 0x3F00B880
https://adamransom.github.io/posts/raspberry-pi-bare-metal-part-1.html

Request Size 0x00 0x00038041‘Set LED’ Tag ID
Request Code 0x04 8Value Buffer Size
0x08 0x0 Request Code
0x0c 130 Power LED Pin Number
0x10 0x1 LED Status (on)

The tag id to turn on or off one of the onboard LEDs is 0x00038041
two 32byte params
    put, in order
    in the value buffer (offset 0x0c) of the tag buffer
