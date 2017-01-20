#include <iostream>
#include <stdio.h>
#include <assert.h>

#define DEF_CAP 8

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
  unsigned int cap;     // Total cap
  unsigned int start_i; // Index of first element
  unsigned int offset;  // Circular distance from start (Can be more than cap)
  MyClass *data;

  unsigned int (*size)(Deque_MyClass*);
  bool (*empty)(struct Deque_MyClass*);

  void (*push_front)(Deque_MyClass*, MyClass);
  void (*push_back)(Deque_MyClass*, MyClass);
  void (*pop_front)(Deque_MyClass*);
  void (*pop_back)(Deque_MyClass*);

  MyClass &(*at)(Deque_MyClass*, unsigned int);
  MyClass &(*front)(Deque_MyClass*);
  MyClass &(*back)(Deque_MyClass*);

  void (*clear)(Deque_MyClass*);
  void (*dtor)(Deque_MyClass*);

  void (*sort)(Deque_MyClass*, Deque_MyClass_Iterator, Deque_MyClass_Iterator);
};

/* Function Definitions */
unsigned int size(Deque_MyClass *deq) {
  return deq->offset - deq->start_i;
}

bool empty(Deque_MyClass *deq) {
  return deq->size(deq) == 0;
}

void push_front(Deque_MyClass *deq, MyClass entry) {
  if(deq->offset - deq->start_i == deq->cap) {
    // TODO Resize here
    return;
  }
  deq->data[(deq->start_i + deq->cap - 1) % deq->cap] = entry;
  deq->start_i = (deq->start_i + deq->cap - 1) % deq->cap;
}

void push_back(Deque_MyClass *deq, MyClass entry) {
  if(deq->offset - deq->start_i == deq->cap) {
    // TODO Resize here
    return;
  }
  deq->data[(deq->start_i + deq->offset) % deq->cap] = entry;
  deq->offset++;
}

void pop_front(Deque_MyClass *deq) {
  deq->start_i = (deq->start_i + 1) % deq->cap;
}

void pop_back(Deque_MyClass *deq) {
  deq->offset--;
}

MyClass &at(Deque_MyClass *deq, unsigned int index) {
  return deq->data[deq->start_i + index];
}

MyClass &front(Deque_MyClass *deq) {
  return deq->data[deq->start_i];
}

MyClass &back(Deque_MyClass *deq) {
  return deq->data[deq->start_i + deq->offset - 1];
}

void clear(Deque_MyClass *deq) {
  deq->start_i = 0;
  deq->offset = 0;

}

//void dtor(Deque_MyClass *deq) {
//}

//void sort(Deque_MyClass *deq, Deque_MyClass_Iterator begin, Deque_MyClass_Iterator end) {
//}

void Deque_MyClass_ctor(Deque_MyClass* deq) {
  deq->cap = DEF_CAP;
  deq->start_i = 0;
  deq->offset = 0;
  deq->data = (MyClass*) malloc(deq->cap * sizeof(Deque_MyClass));

  deq->size = &size;
  deq->empty = &empty;

  deq->push_front = &push_front;
  deq->push_back = &push_back;
  deq->pop_front = &pop_front;
  deq->pop_back = &pop_back;

  deq->at = &at;
  deq->front = &front;
  deq->back = &back;

  deq->clear = &clear;
  //deq->dtor = &dtor;

  //deq->sort = &sort;
}


/* Testing */
int main() {

  MyClass mc = MyClass{1, "Joe"};
  MyClass_print(&mc);

  Deque_MyClass deq;
  Deque_MyClass_ctor(&deq);

  assert(deq.size(&deq) == 0);
  assert(deq.empty(&deq));

  deq.push_back(&deq, MyClass{1, "Joe"});
  deq.push_back(&deq, MyClass{2, "Mary"});
  deq.push_back(&deq, MyClass{3, "Tom"});
  deq.push_front(&deq, MyClass{0, "Mike"});
  deq.push_front(&deq, MyClass{-1, "Mary"});

  MyClass_print(&deq.front(&deq));
  MyClass_print(&deq.back(&deq));
  assert(deq.front(&deq).id == -1);
  assert(deq.back(&deq).id == 3);

  deq.pop_front(&deq);
  deq.pop_back(&deq);
  assert(deq.front(&deq).id == 0);
  assert(deq.back(&deq).id == 2);

  assert(deq.size(&deq) == 3);
}
