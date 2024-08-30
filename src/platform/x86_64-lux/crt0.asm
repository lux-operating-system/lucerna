; lucerna - an implementation of the standard C library
; Omar Elghoul, 2024
;
; Platform-specific code for x86_64 lux

[bits 64]

section .text

; Generic entry point for an application
; The kernel jumps to this function after exec() or equivalent
; Declaration: void _start(const char **argv, const char **env);
; This will parse the arguments and environment, setting them up in the format
; defined by the C standard, and then call the C function main(), and then call
; exit() with the return value from main

global _start
_start:
    ; TODO: actually implement the functionality mentioned above
    
    extern main
    call main

    ; TODO: implement this syscall in the kernel too
    mov rdi, rax        ; return value from main()
    xor rax, rax        ; exit()
    syscall
