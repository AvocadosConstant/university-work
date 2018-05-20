#ifndef CS540_LIST_HPP
#define CS540_LIST_HPP

#include <cstddef>
#include <iostream>
#include <cassert>

template <typename T>
class List {
  public:
    struct S_Node;
    struct Node;
    struct Iterator;

    S_Node *head = {head};

    List() {
      std::cout << "Default ctor" << std::endl;
    }

    ~List() {
      S_Node* cur = head->next;
      while(cur != head) {
        S_Node* tmp = cur->next;
        delete cur;
        cur = tmp;
      }
    }

    Iterator begin() { return Iterator(head->next); }
    Iterator end() { return Iterator(head); }

    void push_front(const T &elem) {
      Node *tmp = new Node(elem);
      tmp->next = head->next;
      head->next = tmp->next;
    }
    void push_back(const T &elem) {
      Node *tmp = new Node(elem);

      S_Node* cur = head->next;
      while(cur->next != head) {
        cur = cur->next;
      }
      cur->next = tmp;
      tmp->next = head;
    }

    Iterator insert(Iterator pos, const T &elem) {assert(false);}

    void erase(Iterator pos) {assert(false);}

    struct S_Node {
      S_Node *next;
    };

    struct Node : public S_Node {
      T data;
      Node(T seed) : data{seed} {std::cout << "Constructing Node with seed value" << std::endl;}
    };

    struct Iterator {
      S_Node *data;

      Iterator() = delete;
      Iterator(S_Node *seed) { this->data = seed; }

      Iterator& operator++() {
        //data = data->right_links[0];
        data = data->next;
        return *this;
      };

      Iterator operator++(int) {
        Iterator tmp(*this);
        operator++();
        return tmp;
      }

      T &operator*() const {
        return ((Node*) data)->data;
      }
    };

  friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
    return lhs.data != rhs.data;
  }

}; // List

#endif
