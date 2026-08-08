;; irc its just like:
[bits 32]

extern isr_pit
global int32

int32:
    pusha
    call isr_pit
    popa
    iretd