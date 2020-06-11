int pow(int x, int y) {}

void test() {
  pow(0,-1); // expected warn: Pole error: if first argument is 0, second argument must be positive
}
