; For the MASM and zero-extending tutorial at https://www.youtube.com/watch?v=I8TL2BbKnbQ
; Build: ml64  /Zi zeroextend.asm /link /subsystem:console /entry:main /DEBUG
.CODE
main PROC
    mov rax,-1          ; loads rax with 0xFFFFFFFFFFFFFFFF
    mov al,1            ; loads the lowest byte of rax (the al reg) with 0x01
    mov ah,2            ; loads the second byte of rax (the ah reg) with 0x02
    mov ax,6543h        ; loads the lowest 16-bits of rax (the ax reg) with 0x6543
    mov eax,0edcba987h  ; loads the lowest 32-bits of rax (the eax reg) with 0xedcba987
                        ; ...but zero-extends to the highest 32-bits...so...
                        ; ...this effectively performs a "mov rax,0edcba987h
    xor eax,eax         ; return 0 for process exit code (success).
    ret                 ; return to caller (kernel user-mode code, will end process with exit code in eax register).
main ENDP
END
