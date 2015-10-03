.globl      _main,_prints,_NSEC                        !Import from C
.globl      _getc,_putc,_readfd,_setes,_inces,_error  !Export to C
            BOOTSEG   = 0x9800  !booter segment
            OSSEG     = 0x1000  !kernel segment
            SSP       = 32*1024 !booter stack size=32kb
            BSECTORS  = 2       !number of sectors to load initially

start:
            mov   ax, #BOOTSEG  !set ES to 0x9800
            mov   es, ax

            !call INT 13 to load BSECTORS to (segment, offset)=(0x9800,0)
            xor   dx, dx        ! dh=head=0, dl=drive=0
            xor   cx, cx        ! ch=cyl=0,  cl=sector=0
            incb  cl            ! sectpr = 1, BIOS counts sector from 1
            xor   bx, bx        ! (ES, BX) = real address = (0x9800, 0)
            movb  ah, #2        ! ah = READ
            movb  al, #BSECTORS ! al = total sectors count to load
            int   0x13          ! call disk I/O

            !far jump to (0x9800, next) to continue execution there
            jmpi next, BOOTSEG  ! CS=BOOTSEG, IP = next

next:
            mov   ax, cs        ! set cpu segment registers to 0x9800
            mov   ds, ax        ! set data segment
            mov   ss, ax        ! set stack segment
            mov   sp, #SSP      ! SP = SS + 32kb
            call  _main
            jmpi  0, OSSEG

_getc:                          ! char getc()
            xorb  ah, ah
            int   0x16
            ret

_putc:                          ! putc(char c): print a char
            push  bp
            mov   bp, sp
            movb  al, 4[bp]     ! al = char
            movb  ah, #14
            mov   bx, #0x000C   ! red colour
            int   0x10
            pop   bp
            ret

_readfd:                        ! readfd(cyl, head, sector)
            push  bp
            mov   bp, sp
            movb  dl, #0x00     ! drive=FD=0
            movb  dh, 6[bp]     ! head
            movb  cl, 8[bp]     ! sector
            incb  cl            ! inc sector by 1 to suit BIOS
            movb  ch, 4[bp]     ! cyl
            xor   bx, bx
            movb  ah, #0x02     ! READ
            movb  al, _NSEC     ! read _NSEC sectors to (ES, BX)
            int   0x13
            jb    _error
            pop   bp
            ret


_setes:                         ! setes(segment): set ES to a segment
            push  bp
            mov   bp, sp
            mov   ax, 4[bp]
            mov   es, ax
            pop   bp
            ret

_inces:                         ! inces(): increment ES by _NSEC sectors (in 16-byte times/clicks)
            mov   bx, _NSEC     ! get _NSEC in BX
            shl   bx, #5        ! multiply by 2**5=32
            mov   ax, es
            add   ax, bx        ! add (_NSEC*32)
            mov   es, ax
            ret

_error:
            push  #msg
            call  _prints
            int   0x19

msg:        .asciz  "Error"
