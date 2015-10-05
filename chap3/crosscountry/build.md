as86 -o bs.o bs.s
bcc -c -ansi bc.c
ld86 -d -o booter bs.o bc.o /usr/lib/bcc/libc.a
dd if=booter of=fdimage.img bs=512 count=1 conv=notrunc
dd if=zImage2.4 of=fdimage.img bs=512 seek=1 conv=notrunc
qemu-system-x86_64 -fda fdimage.img --no-fd-bootchk
