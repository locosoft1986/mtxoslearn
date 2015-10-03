int g = 100;
int h;
static int s;
main(int argc, char *argv[])
{
  int a = 1;
  int b;
  static int c = 3;
  b = 2;
  c = mysum(a,b);
  printf("sum=%d\n", c);
}
