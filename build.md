as86 -o bs.o bs.s
bcc -c -ansi bc.c
ld86 -d -o booter bs.o bc.o /usr/lib/bcc/libc.a
sudo dd if=booter of=/dev/fd0 bs=512 count=1 conv=notrunc
