int sqrt(int a) {}

void test(int a) {
  if(a>=0)
    sqrt(a); // no warn expected
}
