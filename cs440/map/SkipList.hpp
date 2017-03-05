#ifndef _SKIPLIST_HPP_
#define _SKIPLIST_HPP_

#include <iostream>

namespace cs540 {
  template <typename Key_T, typename Mapped_T> class SkipList {
    typedef std::pair<Key_T, Mapped_T> ValueType;

    struct SNode {
      SNode *next, *prev;
    };

    struct Node : public SNode {
      ValueType data;

      Node() : SNode() {}
      Node(Key_T key, Mapped_T value) : data{key, value} {}
      Node(ValueType data) : data{data} {}
    };

    public:
      SNode *head;

      SkipList() {
        head = new SNode();
        head->next = head;
        head->prev = head;
        std::cout << "Constructed" << std::endl;
      }

      // TODO Replace return type with std::pair<Iterator, bool> 
      void insert(const ValueType &element) {
        Node *tmp = new Node(element);
        tmp->next = head;
        head->prev->next = tmp;
        tmp->prev = head->prev;
        head->prev = tmp;
      }

      /*
      Iterator find(const Key_T &);

      ConstIterator find(const Key_T &) const;

      Mapped_T &at(const Key_T &) const;

      const Mapped_T &at(const Key_T &) const;
      */

      void print() {
        for (SNode *p = head->next; p != head; p = p->next) {
          std::cout << ((Node*)p)->data.second << std::endl;
        }
      }
  };
}

#endif
