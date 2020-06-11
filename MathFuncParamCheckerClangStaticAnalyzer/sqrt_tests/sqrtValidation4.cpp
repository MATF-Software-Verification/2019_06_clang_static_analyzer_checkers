int sqrt(int a) {}

void test(int a) {
  sqrt(a); // expected warn: Undefined function argument, could be negative
}
