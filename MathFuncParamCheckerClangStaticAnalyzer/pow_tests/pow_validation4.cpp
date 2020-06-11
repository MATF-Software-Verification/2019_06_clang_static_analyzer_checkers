int pow(int a, int b) {}

void test() {
  int a = 5;
  int b = 12;

  pow(a,b); // no warn expected
}
