; For Intro to Building/Debugging Ghidra Internals Tutorial
; Link to video: https://www.youtube.com/watch?v=G-uoTr4-aPo
;
; To build:
;   ml64.exe /Zi .\InitUninitDataSection.asm /link /subsystem:console /entry:main /DEBUG

MYSTRUCT STRUCT 16
    field db 152 DUP(?)
MYSTRUCT ENDS

.DATA
my_init_var db 200 DUP(5)

.DATA?
my_uninit_var MYSTRUCT<>

.CODE
main PROC
    sub rsp,28h                                     ; Allocate shadow space
    mov rax,offset my_init_var
    mov BYTE PTR [rax],0aah
    mov rax,offset my_uninit_var
    mov BYTE PTR [rax],0bbh
    add rsp,28h  ; Deallocate shadow space
    xor eax,eax                                     ; exit code is 0 success.
    ret
main ENDP
END
