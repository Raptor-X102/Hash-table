BITS 64
global main
global Strcmp_256
extern  ExitProcess

section .text
;------------------------------------------------------------------------------
; Strcmp 256 bits
; Entry: rdi, rsi - strings
; Exit: eax - compare mask
; Destr: eax, ymm0, ymm1, ymm2
;------------------------------------------------------------------------------
Strcmp_256:

    vmovdqu ymm0, [rdi]
    ;vmovdqu ymm1, [rsi]

    vpcmpeqb ymm2, ymm0, [rsi]
    vpmovmskb eax, ymm2
    vzeroupper
    ret
;------------------------------------------------------------------------------
