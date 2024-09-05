; lucerna - an implementation of the standard C library
; Omar Elghoul, 2024
;
; Platform-specific code for x86_64 lux

[bits 64]

section .text

; Generic syscall wrapper for x86_64
; uint64_t luxSyscall(uint64_t request, uint64_t p0, uint64_t p1, uint64_t p2, uint64_t p3)
global luxSyscall
align 16
luxSyscall:
    mov rax, rdi        ; request
    mov rdi, rsi        ; p0
    mov rsi, rdx        ; p1
    mov rdx, rcx        ; p2

    ; r8 is already set to p3
    ; rcx will be trashed by the syscall instruction, which is why we have to
    ; skip it and violate the System V ABI in this one function
    ;
    ; so essentially rax=request, rdi=p0, rsi=p1, rdx=p2, r8=p3

    syscall
    ret
