typedef unsigned char  u8;
typedef unsigned short u16;

#define TRK  18
#define CYL  36

u16 tsize, dsize;
int ksectors, i,  NSEC = 1;

int prints(char *s)
{
  while(*s)
  {
    putc(*s++);
  }
}

int getsector(u16 sector, char *buf)
{
  readfd(sector/CYL, ((sector)%CYL)/TRK, (((sector)%CYL)%TRK), buf);
}


u16 buf[256];

main()
{
  prints("Booting MTX\n\r");
  tsize = *(u16 *)(512 + 2);
  dsize = *(u16 *)(512 + 4);
  ksectors = ((tsize << 4) + dsize + 511)/512;
  setes(0x1000);

  for (i = 1; i <= ksectors + 1; ++i)
  {
    getsector(i);
    inces();
    putc('.');
  }

  prints("\n\rReady to go?");
  getc();
}
