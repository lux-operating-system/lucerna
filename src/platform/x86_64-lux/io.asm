; lucerna - an implementation of the standard C library
; Omar Elghoul, 2024
;
; Platform-specific code for x86_64 lux

[bits 64]

section .text

; void outb(uintptr_t port, uint8_t data)
global outb
align 16
outb:
    mov dx, di
    mov eax, esi
    out dx, al
    ret

; void outw(uintptr_t port, uint16_t data)
global outw
align 16
outw:
    mov dx, di
    mov eax, esi
    out dx, ax
    ret

; void outd(uintptr_t port, uint32_t data)
global outb
align 16
outd:
    mov dx, di
    mov eax, esi
    out dx, eax
    ret

; uint8_t inb(uintptr_t port)
global inb
align 16
inb:
    mov dx, di
    in al, dx
    ret

; uint16_t inw(uintptr_t port)
global inw
align 16
inw:
    mov dx, di
    in ax, dx
    ret

; uint32_t ind(uintptr_t port)
global ind
align 16
ind:
    mov dx, di
    in eax, dx
    ret
