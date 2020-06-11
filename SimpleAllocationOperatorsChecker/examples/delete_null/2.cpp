class A{};

void test_2_helper(A* obj){
  if(obj != null){
    delete obj;
  }
}

void test_2(){
  A* obj = nullptr;

  test_2_helper(obj); // Shouldn't generate warning
}
