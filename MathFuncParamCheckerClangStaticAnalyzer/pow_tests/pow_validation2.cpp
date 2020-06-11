int pow(int a, int b) {}

void test() {
  int a = 0;
  int b = -1;

  pow(a,b); // expected warn: Pole error: if first argument is 0, second argument must be positive
}
