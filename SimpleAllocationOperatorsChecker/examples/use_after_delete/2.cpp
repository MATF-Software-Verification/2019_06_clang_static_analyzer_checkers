class A{
public:
  int field = 42;
  void someFunction(){}
};

void test_2(){
  A* obj = new A();
  delete obj;

  obj->someFunction(); // This should generate warning: "Object used after delete."
}
