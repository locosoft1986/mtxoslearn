        .global _h
        .global _getbp,_mysum,_g
_getbp:
        mov ax,bp
        ret
_mysum:
        push bp
        mov bp,sp
        mov ax,4[bp]
        add ax,6[bp]
        add ax,_h
        mov sp, bp
        pop bp
        ret
_g:     .word 123

