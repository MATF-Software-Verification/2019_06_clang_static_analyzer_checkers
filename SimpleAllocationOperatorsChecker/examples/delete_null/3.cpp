class A{};

void test_3_helper(A* obj){
  if(obj == null){
    delete obj;
  }
}

void test_3(){
  A* obj = nullptr;

  test_3_helper(obj); // Should generate warning "Delete called on a null."
}
