#include <stdio.h>

main()
{
    int t1 = 6383;
    unsigned short t2 = 6383;

    int t3 = t1 << 4;
    unsigned short t4 = t2 << 4;

    printf("t1 = %d\n", (short)t3 << 4);
    printf("t2=%d\n", t4 << 4);
}
