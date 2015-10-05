typedef unsigned char  u8;
typedef unsigned short u16;

#define TRK  18
#define CYL  36

u8 setup;
u16 ksectors, i,  NSEC = 1, INC=0x20;

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
  inces();
}


u8 buf[512];

main()
{
  prints("Booting MTX\n\r");
  getsector(1, buf);
  setup = *(u8 *)&buf[497];
  ksectors = (*(u16 *)&buf[500]) >> 5;
  setes(0x9000);

  for (i = 1; i <= setup + 1 + ksectors + 1; ++i)
  {
    getsector(i, 0);
    i <= setup ? putc('*') : putc('.');
    if (i == setup + 1)
    {
      setes(0x1000);
    }
  }

  prints("\n\rReady to go?");
  getc();
}
