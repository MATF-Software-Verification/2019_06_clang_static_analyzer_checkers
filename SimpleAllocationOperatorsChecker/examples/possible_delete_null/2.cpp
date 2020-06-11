class A{};

void test_2(A* obj, bool someCondition){
  if(someCondition){
    delete obj; // Should generate warning: "There's possibility of calling delete on a null."
  }
}
