int pow(int x, int y) {}

void test(int a, int b) {
  pow(a,b); // expected warn: First pow argument is undefined, could cause domain or range error
}
