class A{}; 

void test_6(){
  if(0){
    A* obj = new A(); // This shouldn't generate warning
  }
}
