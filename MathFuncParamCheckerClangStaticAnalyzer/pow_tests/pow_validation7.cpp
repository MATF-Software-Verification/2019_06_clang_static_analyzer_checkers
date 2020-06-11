int pow(int x, int y) {}

int fn(int& a) { a++;}
void test(int b) {
  int a = 0;
  fn(a);
  pow(a,b); // no warn expected
}
