#ifndef CS540_LIST_HPP
#define CS540_LIST_HPP

template <typename T>
class List {
  public:
    struct S_Node;
    struct Node;
    struct Iterator;

    S_Node *head;

    List() {
      //head = new S_Pillar();
      head->next = head;
    }

    Iterator begin() { return Iterator(head->next); }
    Iterator end() { return Iterator(head); }

    void push_front(const T &elem) {}
    void push_back(const T &elem) {}

    Iterator insert(Iterator pos, const T &elem) {}

    void erase(Iterator pos) {}

    struct S_Node {
      ~S_Node() {}
      S_Node* next;
    };

    struct Node : public S_Node {
      T data;

      Node() : S_Node() {}
      Node(T seed) : data{seed} {}
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
        return ((Node *)data)->data;
      }
    };

  friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
    return lhs.data != rhs.data;
  }

}; // List

#endif
