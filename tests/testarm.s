.section .data
message:
    .ascii "Hello, ARM64!\n"
message_len = 12
.section .text
.global _start
_start:
    mov x0, 1
    ldr x1, =message
    ldr x2, =message_len
    mov x8, 64
    svc 0
    mov x0, 0
    mov x8, 93
    svc 0
