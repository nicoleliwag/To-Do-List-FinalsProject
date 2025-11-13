; count_msabi.asm
; Windows x64 calling convention (first integer arg in ecx)
; Exports: count_tasks
section .text
    global count_tasks

; int count_tasks(int n)
count_tasks:
    mov eax, ecx
    ret
