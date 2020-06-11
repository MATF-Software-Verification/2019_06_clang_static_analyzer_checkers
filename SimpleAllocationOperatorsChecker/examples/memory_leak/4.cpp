class A{};

void test_4_helper(A* obj){
  delete obj;
}

void test_4(){
  test_4_helper(new A()); // This shouldn't generate warning
}

