class A{};

void test_1(A* obj){
  delete obj; // Should generate warning: "There's possibility of calling delete on a null."
}
