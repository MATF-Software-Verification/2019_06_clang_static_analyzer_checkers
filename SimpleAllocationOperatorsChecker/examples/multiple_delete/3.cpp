class A{};

void test_3_helper(A* obj){
  delete obj;
}

void test_3(){
  A* obj = new A();
  delete obj;

  test_3_helper(obj); // This should generate warning: "Delete called multiple times on the same memory location."
}
