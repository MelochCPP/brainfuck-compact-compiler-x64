section .data
    tape times 30 db 0

section .text
    global main
    extern putchar
main:
    mov rsi, tape
    add byte [rsi], 10
    jmp loop
loop:
    cmp byte [rsi], 0
    jz loop_end
    inc rsi
    add byte [rsi], 7
    inc rsi
    add byte [rsi], 10
    inc rsi
    add byte [rsi], 3
    sub rsi, 3
    dec byte [rsi]
    jmp loop
loop_end:
    inc rsi
    add byte [rsi], 2
    
    movzx rcx, byte [rsi]
    call putchar

    inc rsi
    inc byte [rsi]

    movzx rcx, byte [rsi]
    call putchar

    add byte [rsi], 7

    movzx rcx, byte [rsi]
    call putchar
    movzx rcx, byte [rsi]
    call putchar

    add byte [rsi], 3

    movzx rcx, byte [rsi]
    call putchar

    inc rsi
    add byte [rsi], 2

    movzx rcx, byte [rsi]
    call putchar

    sub rsi, 2
    add byte [rsi], 15

    movzx rcx, byte [rsi]
    call putchar

    inc rsi

    movzx rcx, byte [rsi]
    call putchar

    add byte [rsi], 3

    movzx rcx, byte [rsi]
    call putchar

    sub byte [rsi], 6

    movzx rcx, byte [rsi]
    call putchar

    sub byte [rsi], 8

    movzx rcx, byte [rsi]
    call putchar

    inc rsi

    inc byte [rsi]

    movzx rcx, byte [rsi]
    call putchar

    xor rax, rax
    leave
    ret