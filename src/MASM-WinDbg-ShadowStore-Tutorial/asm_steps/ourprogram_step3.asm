; For the MASM tutorial at https://www.youtube.com/watch?v=I8TL2BbKnbQ
; ml64  /Zi ourprogram.asm /link /subsystem:console /entry:main /DEBUG
includelib user32.lib

CONST SEGMENT PARA 'const'
howdy_world_msg db "Howdy there, ASM/user32 world!",0
howdy_world_title db "ASM MessageBox Title",0
CONST ENDS

EXTERNDEF MessageBoxA:proc

.CODE
main PROC
    sub rsp,28h                                     ; Allocate shadow space
    xor rcx,rcx                                     ; rcx = NULL HWND
    lea rdx,offset howdy_world_msg                  ; rdx = address of "Howdy there, ASM/user32 world!"
    lea r8,offset howdy_world_title                 ; r8 = address of "ASM MessageBox Title"
    xor r9d,r9d                                     ; r9d = MB_OK (0)
    call MessageBoxA                                ; Display message box, wait for user to click OK.
    add rsp,28h  ; Deallocate shadow space
    xor eax,eax                                     ; exit code is 0 success.
    ret
main ENDP
END
