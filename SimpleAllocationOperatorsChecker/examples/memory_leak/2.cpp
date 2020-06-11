class A{};

void test_2(){
  A* obj = new A();
  delete obj;

  obj = new A(); // This should generate warning : "You haven't released allocated memory."
}
