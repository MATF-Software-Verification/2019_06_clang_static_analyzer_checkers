int pow(int a, float b) {}

void test() {
  int a = -1;
  float b = 1.5;

  pow(a,b); // expected warn: Domain error: if first argument is less than 0, second argument must be an integer
}
