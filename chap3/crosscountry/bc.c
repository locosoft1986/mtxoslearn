typedef unsigned char  u8;
typedef unsigned short u16;

#define TRK  18
#define CYL  36

u16 setup, ksectors,ES;
u16 csector = 1;     // current loading sector
u16 NSEC = 35;       // number of sectors to load
/*
int prints(char *s)
{
  
   while(*s)
     putc(*s++);
  
  //  putc(*s);
}
*/
int getsector(u16 sector, char *buf)
{
   readfd( sector/CYL,((sector)%CYL)/TRK,(((sector)%CYL)%TRK), buf);
   csector += NSEC;
   inces();
}
						\
main()
{
  setes(0x9000);
  getsector(1, 0);       // load Linux's [boot+SETUP] to 0x9000

  // current sector = SETUP's sector count (at offset 512+497) + 2 
  setup   = *(u8 *)(512+497) + 2;
  ksectors = (*(u16 *)(512+500)) >> 5;

  NSEC = CYL - setup; // sectors remain in cylinder 0
  
  setes(0x1000);      // Linux kernel is loaded at segemnt 0x1000
  getsector(setup,0);   // load the remaininig sectors of cylinder 0

  // we are now at begining of cyl#1
  csector = CYL;

  while (csector < ksectors+setup){
                      // try to load by cylinders of 36 sectors each
    ES = getes();     // current ES value

    if ( ((ES + CYL*0x20) & 0xF000) == (ES & 0xF000)){ //still same segment
        NSEC = CYL;         // load a full cylinder 
        getsector(csector,0);
	putc('C');          // show loaded a cylinder
    }
    else{                   // this cyl will cross 64K
        NSEC = 1;
        while( ((ES + NSEC*0x20) & 0xF000) == (ES & 0xF000) )
	{     NSEC++;  putc('s'); }      // number of sectors can still load
        getsector(csector,0);

	NSEC = CYL - NSEC;  // load remaining sectors of this cyl 
        if (NSEC > 0){      // only if has sectors remaining
           getsector(csector,0);
        }
        putc('|'); 
    }
  }
  putc('E'); 
  getc();
}
