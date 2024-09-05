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
align 16
_start:
    mov rdx, rsi    ; rdx = env
    mov rsi, rdi    ; rsi = argv
    xor rdi, rdi    ; TODO: rdi = argc

    extern main     ; int main(int argc, char **argv, char **env)
    call main       ; the env is obviously optional and probably should not be assumed to exist

    mov rdi, rax    ; return value from main()
    xor rax, rax
    syscall         ; exit()

    jmp $           ; this is unreachable but just in case
