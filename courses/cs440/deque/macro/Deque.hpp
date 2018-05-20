#ifndef _DEQUE_H_
#define _DEQUE_H_

#include <assert.h>
#include <cstring>
#include <iostream>
#include <stdio.h>

#define DEF_CAP 8

#define Deque_DEFINE(t)                                                        \
  struct Deque_##t##_Iterator;                                                 \
  struct Deque_##t;                                                            \
  struct Deque_##t##_Iterator {                                                \
    unsigned int index;                                                        \
    Deque_##t *deq;                                                            \
    void (*inc)(Deque_##t##_Iterator *);                                       \
    void (*dec)(Deque_##t##_Iterator *);                                       \
    t &(*deref)(Deque_##t##_Iterator *);                                       \
  };                                                                           \
  struct Deque_##t {                                                           \
    unsigned int cap;                                                          \
    unsigned int start_i;                                                      \
    unsigned int offset;                                                       \
    char type_name[sizeof(#t) + sizeof("Deque_") - 1];                         \
    t *data;                                                                   \
    bool (*comp)(const t &, const t &);                                        \
    int (*compar)(const void *, const void *, void *);                         \
    unsigned int (*size)(Deque_##t *);                                         \
    bool (*empty)(Deque_##t *);                                                \
    void (*resize)(Deque_##t *);                                               \
    void (*push_front)(Deque_##t *, t);                                        \
    void (*push_back)(Deque_##t *, t);                                         \
    void (*pop_front)(Deque_##t *);                                            \
    void (*pop_back)(Deque_##t *);                                             \
    t &(*at)(Deque_##t *, unsigned int);                                       \
    t &(*front)(Deque_##t *);                                                  \
    t &(*back)(Deque_##t *);                                                   \
    Deque_##t##_Iterator (*begin)(Deque_##t *);                                \
    Deque_##t##_Iterator (*end)(Deque_##t *);                                  \
    void (*clear)(Deque_##t *);                                                \
    void (*dtor)(Deque_##t *);                                                 \
    void (*sort)(Deque_##t *, Deque_##t##_Iterator, Deque_##t##_Iterator);     \
  };                                                                           \
                                                                               \
  /* Deque_##t##_Iterator functions */                                         \
  void inc(Deque_##t##_Iterator *it) { it->index++; }                          \
  void dec(Deque_##t##_Iterator *it) { it->index--; }                          \
  t &deref(Deque_##t##_Iterator *it) {                                         \
    return it->deq->at(it->deq, it->index);                                    \
  }                                                                            \
  bool Deque_##t##_Iterator_equal(const Deque_##t##_Iterator &it1,             \
                                  const Deque_##t##_Iterator &it2) {           \
    return (it1.index == it2.index) && (it1.deq == it2.deq);                   \
  }                                                                            \
  void Deque_##t##_Iterator_ctor(Deque_##t##_Iterator *it, Deque_##t *deq,     \
                                 unsigned int index) {                         \
    it->index = index;                                                         \
    it->deq = deq;                                                             \
    it->inc = &inc;                                                            \
    it->dec = &dec;                                                            \
    it->deref = &deref;                                                        \
  }                                                                            \
                                                                               \
  /* Deque_##t functions */                                                    \
  int t##_compar(const void *lhs, const void *rhs, void *comp_v) {             \
    bool (*comp)(const t &, const t &) =                                       \
        (bool (*)(const t &, const t &))comp_v;                                \
    const t *lhs_c = (const t *)lhs;                                           \
    const t *rhs_c = (const t *)rhs;                                           \
    if (comp(*lhs_c, *rhs_c))                                                  \
      return -1;                                                               \
    if (comp(*rhs_c, *lhs_c))                                                  \
      return 1;                                                                \
    return 0;                                                                  \
  }                                                                            \
  unsigned int size(Deque_##t *deq) { return deq->offset - deq->start_i; }     \
  bool empty(Deque_##t *deq) { return deq->size(deq) == 0; }                   \
  void resize(Deque_##t *deq) {                                                \
    t *new_data = (t *)malloc(2 * deq->cap * sizeof(Deque_##t));               \
    for (unsigned int i = 0; i < deq->cap; i++) {                              \
      new_data[i] = deq->data[(i + deq->start_i) % deq->cap];                  \
    }                                                                          \
    deq->start_i = 0;                                                          \
    deq->offset = deq->cap;                                                    \
    deq->cap *= 2;                                                             \
    free(deq->data);                                                           \
    deq->data = new_data;                                                      \
  }                                                                            \
  void push_front(Deque_##t *deq, t entry) {                                   \
    if (deq->offset - deq->start_i == deq->cap) {                              \
      deq->resize(deq);                                                        \
    }                                                                          \
    deq->data[(deq->start_i + deq->cap - 1) % deq->cap] = entry;               \
    deq->start_i = (deq->start_i + deq->cap - 1) % deq->cap;                   \
    if (deq->offset < deq->start_i)                                            \
      deq->offset += deq->cap;                                                 \
  }                                                                            \
  void push_back(Deque_##t *deq, t entry) {                                    \
    if (deq->offset - deq->start_i == deq->cap) {                              \
      deq->resize(deq);                                                        \
    }                                                                          \
    deq->data[deq->offset % deq->cap] = entry;                                 \
    deq->offset++;                                                             \
  }                                                                            \
  void pop_front(Deque_##t *deq) {                                             \
    if (deq->start_i != deq->offset) {                                         \
      deq->start_i = (deq->start_i + 1) % deq->cap;                            \
      if (deq->offset > deq->cap && deq->start_i < deq->offset - deq->cap) {   \
        deq->offset -= deq->cap;                                               \
      }                                                                        \
    }                                                                          \
  }                                                                            \
  void pop_back(Deque_##t *deq) {                                              \
    if (deq->start_i != deq->offset) {                                         \
      if (deq->offset == 0)                                                    \
        deq->offset = deq->cap - 1;                                            \
      else                                                                     \
        deq->offset--;                                                         \
    }                                                                          \
  }                                                                            \
  t &at(Deque_##t *deq, unsigned int index) {                                  \
    return deq->data[(deq->start_i + index) % deq->cap];                       \
  }                                                                            \
  t &front(Deque_##t *deq) {                                                   \
    assert(deq->offset - deq->start_i > 0);                                    \
    return deq->data[deq->start_i];                                            \
  }                                                                            \
  t &back(Deque_##t *deq) {                                                    \
    assert(deq->offset - deq->start_i > 0);                                    \
    return deq->data[(deq->offset - 1) % deq->cap];                            \
  }                                                                            \
  Deque_##t##_Iterator begin(Deque_##t *deq) {                                 \
    Deque_##t##_Iterator it;                                                   \
    Deque_##t##_Iterator_ctor(&it, deq, 0);                                    \
    return it;                                                                 \
  }                                                                            \
  Deque_##t##_Iterator end(Deque_##t *deq) {                                   \
    Deque_##t##_Iterator it;                                                   \
    Deque_##t##_Iterator_ctor(&it, deq, deq->offset - deq->start_i);           \
    return it;                                                                 \
  }                                                                            \
  void clear(Deque_##t *deq) {                                                 \
    deq->start_i = 0;                                                          \
    deq->offset = 0;                                                           \
  }                                                                            \
  void dtor(Deque_##t *deq) { free(deq->data); }                               \
  void sort(Deque_##t *deq, Deque_##t##_Iterator begin,                        \
            Deque_##t##_Iterator end) {                                        \
    unsigned int sort_size = (end.index - begin.index + deq->cap) % deq->cap;  \
    t *tmpArr = (t *)malloc(sort_size * sizeof(Deque_##t));                    \
    for (unsigned int i = 0; i < sort_size; i++) {                             \
      tmpArr[i] = deq->at(deq, (begin.index + i) % deq->cap);                  \
    }                                                                          \
    qsort_r(tmpArr, sort_size, sizeof(t), deq->compar, (void *)deq->comp);     \
    for (unsigned int i = 0; i < sort_size; i++) {                             \
      deq->at(deq, (begin.index + i) % deq->cap) = tmpArr[i];                  \
    }                                                                          \
    free(tmpArr);                                                              \
  }                                                                            \
  bool Deque_##t##_equal(Deque_##t &deq1, Deque_##t &deq2) {                   \
    if (deq1.size(&deq1) != deq2.size(&deq2))                                  \
      return false;                                                            \
    for (unsigned int i = 0; i < deq1.size(&deq1); i++) {                      \
      if (deq1.comp(deq1.at(&deq1, i), deq2.at(&deq2, i)) ||                   \
          deq2.comp(deq2.at(&deq2, i), deq1.at(&deq1, i)))                     \
        return false;                                                          \
    }                                                                          \
    return true;                                                               \
  }                                                                            \
  void Deque_##t##_ctor(Deque_##t *deq, bool (*comp)(const t &, const t &)) {  \
    deq->cap = DEF_CAP;                                                        \
    deq->start_i = 0;                                                          \
    deq->offset = 0;                                                           \
    deq->type_name[0] = '\0';                                                  \
    strcat(deq->type_name, "Deque_");                                          \
    strcat(deq->type_name, #t);                                                \
    deq->data = (t *)malloc(deq->cap * sizeof(t));                             \
    deq->comp = comp;                                                          \
    deq->compar = t##_compar;                                                  \
    deq->size = &size;                                                         \
    deq->empty = &empty;                                                       \
    deq->resize = &resize;                                                     \
    deq->push_front = &push_front;                                             \
    deq->push_back = &push_back;                                               \
    deq->pop_front = &pop_front;                                               \
    deq->pop_back = &pop_back;                                                 \
    deq->at = &at;                                                             \
    deq->front = &front;                                                       \
    deq->back = &back;                                                         \
    deq->begin = &begin;                                                       \
    deq->end = &end;                                                           \
    deq->clear = &clear;                                                       \
    deq->dtor = &dtor;                                                         \
    deq->sort = &sort;                                                         \
  }
#endif
