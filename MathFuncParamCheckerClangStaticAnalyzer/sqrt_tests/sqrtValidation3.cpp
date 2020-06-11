int sqrt(int a) {}

void test() {
  int a = -5;
  if(a>0)
    sqrt(a); // no warn expected
}
