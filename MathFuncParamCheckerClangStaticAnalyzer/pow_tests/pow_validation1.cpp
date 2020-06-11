int pow(int x, int y) {}

void test() {
  int a = -1;
  int b = -1;

  pow(a,b); // no warn expected
}
