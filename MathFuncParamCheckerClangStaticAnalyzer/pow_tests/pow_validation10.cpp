int pow(int x, float y) {}

void test() {
  pow(-1,-1.0); // expected warn: Domain error: if first argument is less than 0, second argument must be an integer
}
