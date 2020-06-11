int sqrt(int a) {}

int fn(int& a) {
  a--;
}

void test() {
  int a = 0;
  fn(a);
  sqrt(a); // expected warn: Function argument is negative, domain error
}
