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
    ; at the start from the kernel, rdi = argv, and rsi = envp

    mov rdx, rsi        ; rdx = envp
    mov rsi, rdi        ; rsi = argv

    xor rdi, rdi        ; argc = 0
    test rsi, rsi       ; null?
    jz .main

    ; nope, count the arguments
    push rsi

.count_args:
    lodsq
    test rax, rax
    jz .counted

    inc rdi             ; increment argc
    jmp .count_args

.counted:
    pop rsi             ; original argv

.main:
    ; store the environmental variables
    extern environ
    mov r8, environ
    mov [r8], rdx

    extern main         ; int main(int argc, char **argv, char **envp)
    call main           ; the env is obviously optional and probably should not be assumed to exist

    extern exit         ; exit(int status) - to allow calling atexit() handlers
    mov rdi, rax        ; return value from main()
    call exit

    jmp $               ; this is unreachable but just in case
