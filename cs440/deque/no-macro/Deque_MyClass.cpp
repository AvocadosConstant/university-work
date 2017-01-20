#include <iostream>
#include <stdio.h>
#include <assert.h>

/* MyClass */
struct MyClass {
  int id;
  char name[10];
};
void MyClass_print(const MyClass *o) {
  printf("%d\n", o->id);
  printf("%s\n", o->name);
}

/* Forward Declarations */
struct Deque_MyClass_Iterator {
  /* TODO */
};

struct Deque_MyClass {
  unsigned int cur_size;    // Number of elems
  unsigned int capacity;    // Total capacity

  unsigned int (*size)(Deque_MyClass*);
  bool (*empty)(struct Deque_MyClass*);

  void (*push_front)(struct Deque_MyClass*, struct MyClass);
  void (*push_back)(struct Deque_MyClass*, struct MyClass);
  MyClass (*pop_front)(struct Deque_MyClass*);
  MyClass (*pop_back)(struct Deque_MyClass*);

  MyClass &(*at)(struct Deque_MyClass*, unsigned int);
  MyClass &(*front)(struct Deque_MyClass*);
  MyClass &(*back)(struct Deque_MyClass*);

  void (*clear)(struct Deque_MyClass*);
  void (*dtor)(struct Deque_MyClass*);

  void (*sort)(struct Deque_MyClass*, struct Deque_MyClass_Iterator, struct Deque_MyClass_Iterator);
};

/* Function Definitions */
void Deque_MyClass_ctor(struct Deque_MyClass* deq) {
  deq->cur_size = 0;
  deq->capacity = 8;
}

unsigned int size(Deque_MyClass *deq) {
  return deq->cur_size;
}

bool empty(Deque_MyClass *deq) {
  return deq->size(deq) == 0;
}

/* Testing */
int main() {

  MyClass mc = MyClass{1, "Joe"};
  MyClass_print(&mc);

  Deque_MyClass deq;
  Deque_MyClass_ctor(&deq);

  //printf("Size: %d", deq.size(&deq));
  assert(deq.size(&deq) == 0);
  //assert(deq.empty(&deq));
}
