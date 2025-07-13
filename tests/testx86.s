.section .data
message:
    .ascii "Hello, x86!\n"
.section .text
.globl _start
_start:
    mov $4, %eax
    mov $1, %ebx
    mov $message, %ecx
    mov $12, %edx
    int $0x80
    mov $1, %eax
    xor %ebx, %ebx
    int $0x80

