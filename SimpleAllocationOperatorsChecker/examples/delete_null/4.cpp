class A{};

void test_4(A* obj){
  if(!(obj = nullptr)){
    delete obj; // Should generate warning: "Delete called on a null."
  }
}
