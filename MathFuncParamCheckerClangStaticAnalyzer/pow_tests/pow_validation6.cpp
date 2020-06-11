int pow(int x, int y) {}

void test(int b) {
  int a = 0;
  pow(a,b); // expected warn: Second pow argument is undefined, could cause domain or range error
}
