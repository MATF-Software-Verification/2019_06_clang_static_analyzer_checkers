int sqrt(int a) {}

void test(int a) {
  if(a<0)
    sqrt(a); // expected warn: Function argument is negative, domain error
}
