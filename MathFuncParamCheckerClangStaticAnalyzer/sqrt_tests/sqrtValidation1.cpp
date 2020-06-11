int sqrt(int a) {}

void test() {
  int a = -5;
  sqrt(a); // expected warn: Function argument is negative, domain error
}
