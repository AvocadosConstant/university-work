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
  printf("ID %d: %s\n", o->id, o->name);
  /*
  printf("%d\n", o->id);
  printf("%s\n", o->name);
  */
}

/* Forward Declarations */
struct Deque_MyClass_Iterator {
  void (*inc)(Deque_MyClass_Iterator*);
  void (*dec)(Deque_MyClass_Iterator*);
  void (*deref)(Deque_MyClass_Iterator*);
};

struct Deque_MyClass {
  unsigned int cap;     // Total cap
  unsigned int start_i; // Index of first element
  unsigned int offset;  // Circular distance from start (Can be more than cap)
  MyClass *data;

  unsigned int (*size)(Deque_MyClass*);
  bool (*empty)(Deque_MyClass*);
  void (*resize)(Deque_MyClass*);

  void (*push_front)(Deque_MyClass*, MyClass);
  void (*push_back)(Deque_MyClass*, MyClass);
  void (*pop_front)(Deque_MyClass*);
  void (*pop_back)(Deque_MyClass*);

  MyClass &(*at)(Deque_MyClass*, unsigned int);
  MyClass &(*front)(Deque_MyClass*);
  MyClass &(*back)(Deque_MyClass*);
  Deque_MyClass_Iterator (*begin)(Deque_MyClass*);
  Deque_MyClass_Iterator (*end)(Deque_MyClass*);

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

void resize(Deque_MyClass *deq) {
  MyClass *new_data = (MyClass*) malloc(2 * deq->cap * sizeof(Deque_MyClass));
  for(unsigned int i = 0; i < deq->cap; i++) {
    new_data[i] = deq->data[(i + deq->start_i) % deq->cap];
  }
  deq->start_i = 0;
  deq->offset = deq->cap;
  deq->cap *= 2;
  free(deq->data);
  deq->data = new_data;
}

void push_front(Deque_MyClass *deq, MyClass entry) {
  // If no more room, then resize
  if(deq->offset - deq->start_i == deq->cap) {
    deq->resize(deq);
  }
  deq->data[(deq->start_i + deq->cap - 1) % deq->cap] = entry;
  deq->start_i = (deq->start_i + deq->cap - 1) % deq->cap;
  if(deq->offset < deq->start_i) deq->offset += deq->cap;
}

void push_back(Deque_MyClass *deq, MyClass entry) {
  // If no more room, then resize
  if(deq->offset - deq->start_i == deq->cap) {
    deq->resize(deq);
  }
  deq->data[deq->offset % deq->cap] = entry;
  deq->offset++;
}

void pop_front(Deque_MyClass *deq) {
  // Make sure there is something to pop
  if(deq->start_i != deq->offset) {
    deq->start_i = (deq->start_i + 1) % deq->cap;
    if(deq->offset > deq->cap && deq->start_i < deq->offset - deq->cap) {
      deq->offset -= deq->cap;
    }
  }
}

void pop_back(Deque_MyClass *deq) {
  // Make sure there is something to pop
  if(deq->start_i != deq->offset) {
    if(deq->offset == 0) deq->offset = deq->cap - 1;
    else deq->offset--;
  }
}

MyClass &at(Deque_MyClass *deq, unsigned int index) {
  return deq->data[(deq->start_i + index) % deq->cap];
}

MyClass &front(Deque_MyClass *deq) {
  assert(deq->offset - deq->start_i > 0);
  return deq->data[deq->start_i];
}

MyClass &back(Deque_MyClass *deq) {
  assert(deq->offset - deq->start_i > 0);
  return deq->data[(deq->offset - 1) % deq->cap];
}

Deque_MyClass_Iterator begin(Deque_MyClass*) {
}

Deque_MyClass_Iterator end(Deque_MyClass*) {
}

void clear(Deque_MyClass *deq) {
  deq->start_i = 0;
  deq->offset = 0;
}

void dtor(Deque_MyClass *deq) {
  free(deq->data);
}

//void sort(Deque_MyClass *deq, Deque_MyClass_Iterator begin, Deque_MyClass_Iterator end) {
//}

void Deque_MyClass_ctor(Deque_MyClass *deq) {
  deq->cap = DEF_CAP;
  deq->start_i = 0;
  deq->offset = 0;
  deq->data = (MyClass*) malloc(deq->cap * sizeof(Deque_MyClass));

  deq->size = &size;
  deq->empty = &empty;
  deq->resize = &resize;

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

void print_Deque(Deque_MyClass *deq) {
  printf("\n______________\n");
  printf("Cap\t\t%d\n"
    "Size\t\t%d\n"
    "Start Index\t%d\n"
    "Offset\t\t%d\n",
    deq->cap, 
    deq->offset - deq->start_i,
    deq->start_i, 
    deq->offset);
  printf("--------------\n");
  for(unsigned int i = 0; i < deq->cap; i++) {
    MyClass_print(&deq->data[i]);
    printf("--------------\n");
  }
  printf("\n");
}

void print_Deque_inOrder(Deque_MyClass *deq) {
  printf("\nPrint in order\n______________\n");
  for(unsigned int i = deq->start_i; i < deq->offset; i++) {
    MyClass_print(&deq->data[i % deq->cap]);
    printf("--------------\n");
  }
  printf("\n");
}

/* Testing */
int main() {
  Deque_MyClass deq;
  Deque_MyClass_ctor(&deq);

  assert(deq.size(&deq) == 0);
  assert(deq.empty(&deq));

  /* My test cases */
  unsigned int max_elems = 1000;

  // Alternate pushing to the front and back
  for(unsigned int i = 0; i < max_elems; i++) {
    if(i % 2 == 0) {
      //printf("Push back %d\n", i);
      deq.push_back(&deq, MyClass{(int) i, "e"});
    } else {
      //printf("Push front %d\n", i);
      deq.push_front(&deq, MyClass{(int) i, "o"});
    }
    //print_Deque(&deq);
  }
  print_Deque_inOrder(&deq);
  assert(deq.size(&deq) == max_elems);

  // Pop everything and verify that it's empty
  printf("____________\nPopping\n____________\n");
  for(unsigned int i = 0; i < max_elems; i++) {
    if(i % 2 == 0) {
      //printf("Pop back %d\n", i);
      deq.pop_back(&deq);
    } else {
      //printf("Pop front %d\n", i);
      deq.pop_front(&deq);
    }
    //print_Deque(&deq);
  }
  assert(deq.size(&deq) == 0);
  assert(deq.empty(&deq));



  /* From prof's test case */
  printf("\n\nProfessor's Test Case\n");
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
