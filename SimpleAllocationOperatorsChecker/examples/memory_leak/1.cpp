class A{};

void test_1(){
  A* obj = new A(); // This should generate warning : "You haven't released allocated memory."
}