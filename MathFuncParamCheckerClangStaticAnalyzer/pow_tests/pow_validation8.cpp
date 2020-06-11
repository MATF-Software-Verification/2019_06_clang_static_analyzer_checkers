int pow(int x, int y) {}

int fn(int& a) { a--;}
void test() {
  int a = 1;
  int b = -1;
  fn(a);
  pow(a,b); // expected warn: Pole error: if first argument is 0, second argument must be positive
}
