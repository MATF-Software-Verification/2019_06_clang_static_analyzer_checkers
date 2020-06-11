class A{
public:
  int field = 42;
  void someFunction(){}
};

void test_1(){
  A* obj = new A();
  delete obj;

  int ft = obj->field; // This should generate warning: "Object used after delete."
}