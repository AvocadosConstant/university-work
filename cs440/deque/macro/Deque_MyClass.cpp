#include <iostream>
#include <stdio.h>
#include <assert.h>

#define DEF_CAP 8

/* MyClass */
struct MyClass {
  int id;
  char name[10];
};

bool MyClass_less_by_id(const MyClass &o1, const MyClass &o2) {return o1.id < o2.id;}

void MyClass_print(const MyClass *o) {
  //printf("ID %d: %s\n", o->id, o->name);
     printf("%d\n", o->id);
     printf("%s\n", o->name);
   /**/
}


//////////////////////////
// Forward Declarations //
//////////////////////////
struct Deque_MyClass_Iterator;
struct Deque_MyClass;

struct Deque_MyClass_Iterator {
  unsigned int index;
  Deque_MyClass *deq;

  void (*inc)(Deque_MyClass_Iterator*);
  void (*dec)(Deque_MyClass_Iterator*);
  MyClass &(*deref)(Deque_MyClass_Iterator*);
};

struct Deque_MyClass {
  unsigned int cap;     // Total cap
  unsigned int start_i; // Index of first element
  unsigned int offset;  // Circular distance from start (Can be more than cap)
  MyClass *data;

  bool (*comp)(const MyClass&, const MyClass&);
  int (*compar)(const void*, const void*, void*);

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



//////////////////////////
// Function Definitions //
//////////////////////////

/* Deque_MyClass_Iterator functions */
void inc(Deque_MyClass_Iterator* it) {it->index++;}

void dec(Deque_MyClass_Iterator* it) {it->index--;}

MyClass &deref(Deque_MyClass_Iterator* it) {return it->deq->at(it->deq, it->index);}

bool Deque_MyClass_Iterator_equal(const Deque_MyClass_Iterator &it1, const Deque_MyClass_Iterator &it2) {
  return (it1.index == it2.index) && (it1.deq == it2.deq);
}

void Deque_MyClass_Iterator_ctor(Deque_MyClass_Iterator *it, Deque_MyClass *deq, unsigned int index) {
  it->index = index;
  it->deq = deq;
  it->inc = &inc;
  it->dec = &dec;
  it->deref = &deref;
}

/* Deque_MyClass functions */
int compar(const void *lhs, const void *rhs, void *comp_v) {
  bool (*comp)(const MyClass&, const MyClass&) = (bool (*)(const MyClass&, const MyClass&))comp_v;
  const MyClass *lhs_c = (const MyClass*)lhs;
  const MyClass *rhs_c = (const MyClass*)rhs;
  if (comp(*lhs_c, *rhs_c)) return -1;
  if (comp(*rhs_c, *lhs_c)) return 1;
  return 0;
}

unsigned int size(Deque_MyClass *deq) {return deq->offset - deq->start_i;}

bool empty(Deque_MyClass *deq) {return deq->size(deq) == 0;}

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

MyClass &at(Deque_MyClass *deq, unsigned int index) {return deq->data[(deq->start_i + index) % deq->cap];}

MyClass &front(Deque_MyClass *deq) {
  assert(deq->offset - deq->start_i > 0);
  return deq->data[deq->start_i];
}

MyClass &back(Deque_MyClass *deq) {
  assert(deq->offset - deq->start_i > 0);
  return deq->data[(deq->offset - 1) % deq->cap];
}

Deque_MyClass_Iterator begin(Deque_MyClass* deq) {
  Deque_MyClass_Iterator it;
  Deque_MyClass_Iterator_ctor(&it, deq, 0);
  return it;
}

Deque_MyClass_Iterator end(Deque_MyClass* deq) {
  Deque_MyClass_Iterator it;
  Deque_MyClass_Iterator_ctor(&it, deq, deq->offset - deq->start_i);
  return it;
}

void clear(Deque_MyClass *deq) {
  deq->start_i = 0;
  deq->offset = 0;
}

void dtor(Deque_MyClass *deq) {free(deq->data);}

void sort(Deque_MyClass *deq, Deque_MyClass_Iterator begin, Deque_MyClass_Iterator end) {
  unsigned int sort_size = (end.index - begin.index + deq->cap) % deq->cap;

  // Copy values to be sorted over to a tmp array so it's contiguous
  MyClass* tmpArr = (MyClass*)malloc(sort_size * sizeof(Deque_MyClass));
  for(unsigned int i = 0; i < sort_size; i++) {
    tmpArr[i] = deq->at(deq, (begin.index + i) % deq->cap);
  }

  qsort_r(tmpArr, sort_size, sizeof(MyClass), deq->compar, (void*)deq->comp);

  // Copy the sorted tmp array elements back into data in their respective positions
  for(unsigned int i = 0; i < sort_size; i++) {
    deq->at(deq, (begin.index + i) % deq->cap) = tmpArr[i];
  }
}

bool Deque_MyClass_equal(Deque_MyClass &deq1, Deque_MyClass &deq2) {
  if(deq1.size(&deq1) != deq2.size(&deq2)) return false;
  for(unsigned int i = 0; i < deq1.size(&deq1); i++) {
    if(deq1.comp(deq1.at(&deq1, i), deq2.at(&deq2, i)) || 
      deq2.comp(deq2.at(&deq2, i), deq1.at(&deq1, i))
    ) return false;
  } 
  return true;
}


void Deque_MyClass_ctor(Deque_MyClass *deq, bool (*comp)(const MyClass&, const MyClass&)) {
  deq->cap = DEF_CAP;
  deq->start_i = 0;
  deq->offset = 0;
  deq->data = (MyClass*) malloc(deq->cap * sizeof(MyClass));

  deq->comp = comp;
  deq->compar = compar;

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
  deq->begin = &begin;
  deq->end = &end;

  deq->clear = &clear;
  deq->dtor = &dtor;

  deq->sort = &sort;
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

  /* From prof's test case */
  Deque_MyClass deq;
  Deque_MyClass_ctor(&deq, MyClass_less_by_id);

  assert(deq.size(&deq) == 0);
  assert(deq.empty(&deq));

  //// Should print "---- Deque_MyClass, 14".
  //printf("---- %s, %d\n", deq.type_name, (int) sizeof(deq.type_name));
  //// std::cout << "---- " << deq.type_name << ", " << sizeof(deq.type_name) << std::endl;
  //assert(sizeof deq.type_name == 14);

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

  //print_Deque(&deq);
  //print_Deque_inOrder(&deq);

  assert(deq.size(&deq) == 3);

  // Testing iterators
  for (Deque_MyClass_Iterator it = deq.begin(&deq); 
    !Deque_MyClass_Iterator_equal(it, deq.end(&deq)); it.inc(&it)) {
    MyClass_print(&it.deref(&it));
  }
  
  //print_Deque(&deq);
  //print_Deque_inOrder(&deq);
  
  //assert(false);
  // Multiple iterators?
  for (Deque_MyClass_Iterator it1 = deq.begin(&deq);
      !Deque_MyClass_Iterator_equal(it1, deq.end(&deq)); it1.inc(&it1)) {
    MyClass_print(&it1.deref(&it1));
    for (Deque_MyClass_Iterator it2 = deq.begin(&deq);
        !Deque_MyClass_Iterator_equal(it2, deq.end(&deq)); it2.inc(&it2)) {
      MyClass_print(&it2.deref(&it2));
      for (Deque_MyClass_Iterator it3 = deq.begin(&deq);
          !Deque_MyClass_Iterator_equal(it3, deq.end(&deq)); it3.inc(&it3)) {
        MyClass_print(&it3.deref(&it3));
      }
    }
  }

  // Test decrement of end.
  {
    auto it = deq.end(&deq);
    it.dec(&it);
    assert(it.deref(&it).id == 2);
  }

  // printf("Using at.\n");

  for (size_t i = 0; i < 3; i++) {
    MyClass_print(&deq.at(&deq, i));
  }

  // Test that front(), back(), at(), and deref() are returning a reference.
  // Change via at().
  assert(deq.at(&deq, 0).id == 0);
  deq.at(&deq, 0).id = 100;
  assert(deq.at(&deq, 0).id == 100);
  // Change via front().
  assert(deq.front(&deq).id == 100);
  deq.front(&deq).id = 0;
  assert(deq.front(&deq).id == 0);
  assert(deq.at(&deq, 0).id == 0); // Verify with at() also.
  {
    auto it = deq.end(&deq);
    it.dec(&it);
    assert(it.deref(&it).id == 2);
    it.deref(&it).id = 200;
    assert(it.deref(&it).id == 200);
    // Change using back().
    assert(deq.back(&deq).id == 200);
    deq.back(&deq).id = 2;
    assert(deq.back(&deq).id == 2);
    assert(it.deref(&it).id == 2); // Verify with iterator also.
  }

  deq.clear(&deq);
  deq.dtor(&deq);

  // Test equality.  Two deques compare equal if they are of the same
  // length and all the elements compare equal.  It is undefined behavior
  // if the two deques were constructed with different comparison
  // functions.
  {
    Deque_MyClass deq1, deq2;
    Deque_MyClass_ctor(&deq1, MyClass_less_by_id);
    Deque_MyClass_ctor(&deq2, MyClass_less_by_id);

    deq1.push_back(&deq1, MyClass{1, "Joe"});
    deq1.push_back(&deq1, MyClass{2, "Jane"});
    deq1.push_back(&deq1, MyClass{3, "Mary"});
    deq2.push_back(&deq2, MyClass{1, "Joe"});
    deq2.push_back(&deq2, MyClass{2, "Jane"});
    deq2.push_back(&deq2, MyClass{3, "Mary"});

    assert(Deque_MyClass_equal(deq1, deq2));

    deq1.pop_back(&deq1);
    assert(!Deque_MyClass_equal(deq1, deq2));
    deq1.push_back(&deq1, MyClass{4, "Mary"});

    assert(!Deque_MyClass_equal(deq1, deq2));

    deq1.dtor(&deq1);
    deq2.dtor(&deq2);
  }
  {
    Deque_MyClass deq;
    Deque_MyClass_ctor(&deq, MyClass_less_by_id);

    deq.push_back(&deq, MyClass{9, "Joe"});
    deq.push_back(&deq, MyClass{2, "Mary"});
    deq.push_back(&deq, MyClass{-20, "Tom"});
    deq.push_front(&deq, MyClass{0, "Mike"});
    deq.push_front(&deq, MyClass{-4, "Mary"});

    deq.sort(&deq, deq.begin(&deq), deq.end(&deq));
    
    print_Deque_inOrder(&deq);
  }
}
