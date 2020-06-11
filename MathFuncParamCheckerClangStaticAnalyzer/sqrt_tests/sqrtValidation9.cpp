int sqrt(int a) {}

int fn(int& a) {
  a++;
}

void test() {
  int a = -1;
  fn(a);
  sqrt(a); // no warn expected
}
