movz w10, #0
movz w11, #1
movz w9, #1

movz w4, #0

start:


updateOnRequest:
    ldr w0, messageAddress
    
    movz w1, #32
    movz w6, #0
    str w1, [x0, w6]

    movz w1, #0
    movz w6, #4
    str w1, [x0, w6]

    ldr w1, tagid
    movz w6, #8
    str w1, [x0, w6]

    movz w1, #8
    movz w6, #12
    str w1, [x0, w6]

    movz w1, #0
    movz w6, #16
    str w1, [x0, w6]

    movz w1, #130
    movz w6, #20
    str w1, [x0, w6]

    movz w6, #24
    str w9, [x0, w6]

    movz w1, #0
    movz w6, #28
    str w1, [x0, w6]

writeCheck:
    ldr w5, statusWriteAddress
    ldr w5, [w5]
    movz w3, #0x8000, lsl #16
    tst w5, w3
    b.ne writeCheck


writeToWriteRegister:
    ldr w0, messageAddress
    ldr w2, ledChannel
    orr w0, w0, w2
    ldr w1, writeAddress
    str w0, [x1, w10]
    add w4, w4, #1

movz w7, #0xf, lsl #16
delay:
    subs w7, w7, #1
    b.ne delay

readCheck:
    ldr w5, statusReadAddress
    ldr w5, [x5]
    movz w3, #0x4000, lsl #16
    ands wzr, w3, w5
    b.ne readCheck

    ldr w0, readAddress
    ldr w1, [x0]

    eor w9, w9, w11

    cmp w9, #0

    b start


ledChannel:
    .int 8

messageAddress:
    .int 0x81000

readAddress:
    .int 0x3f00b880

writeAddress:
    .int 0x3f00b8a0

statusAddress:
    .int 0x3f00b8b8

tagid:
    .int 0x00038041


statusReadAddress:
    .int 0x3f00b898