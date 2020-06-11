class A{};

void test_1(){
  A* obj = new A();
  delete obj;

  delete obj; // This should generate warning: "Delete called multiple times on the same memory location."
}
