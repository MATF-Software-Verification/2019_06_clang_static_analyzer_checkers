class A{};

void test_5(){
  for(int i = 0; i < 42; i++){
    A* obj = new A();	// This should generate warning : "You haven't released allocated memory."
  }
}
