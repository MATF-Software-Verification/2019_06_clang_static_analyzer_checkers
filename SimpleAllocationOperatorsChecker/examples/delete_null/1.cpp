class A{};

void test_1(){
  A* obj = nullptr;

  delete obj; // Should generate warning: "Delete called on a null"
}
