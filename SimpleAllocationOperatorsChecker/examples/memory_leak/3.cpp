class A{};

void test_3(){
  new A(); // This should generate warning : "You haven't released allocated memory."
}

