#ifndef _MAP_HPP_
#define _MAP_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <utility>
#include <vector>
#include <initializer_list>

namespace cs540 {
  template <typename Key_T, typename Mapped_T> class Map {
    typedef std::pair<Key_T, Mapped_T> ValueType;

    public:
      struct Iterator;
      struct ConstIterator;
      struct ReverseIterator;
      struct S_Pillar;
      struct Pillar;

      S_Pillar *head;
      std::size_t height;
      std::size_t num_elements;
      static const std::size_t DEFAULT_HEIGHT = 16;

      Map() {
        num_elements = 0;
        head = new S_Pillar();
        height = DEFAULT_HEIGHT;
        for(std::size_t i = 0; i < height; i++) {
          head->right_links.push_back(head);
          head->left_links.push_back(head);
        }
        // TODO Remove seed before submitting
        srand(time(NULL));
      }

      ~Map() {
        for(S_Pillar *p = head->right_links[0]->right_links[0]; p != head && p->right_links[0] != NULL; p = p->right_links[0]) {
          delete p->left_links[0];
        }
        if(head->left_links[0] != head) delete head->left_links[0];
        delete head;
      }

      size_t size() const { return num_elements; }

      bool empty() const { return num_elements == 0; }

      Iterator begin() { return Iterator(head->right_links[0]); }

      Iterator end() { return Iterator(head); }

      Iterator find(const Key_T &target) {
        S_Pillar *cur = head;
        int level = height - 1;

        while(level >= 0) {
          if(cur->right_links[level] == head || ((Pillar *)cur->right_links[level])->data.first > target) {
            // Go 1 level down
            level--;
          } else {
            // Go right
            cur = cur->right_links[level];
          }
          // Did we find the target key?
          if(level >= 0 && cur != head && ((Pillar *)cur)->data.first == target) {
            return Iterator(cur);
          }
        }
        return this->end();
      }

      ConstIterator find(const Key_T &) const;

      Mapped_T &at(const Key_T & target) {
        Iterator it = find(target);
        if(it.data == head) throw std::out_of_range("Could not find value!");
        return it->second;
      }

      const Mapped_T &at(const Key_T &) const;

      std::pair<Iterator, bool> insert(const ValueType &element) {
        // The pillar to be inserted
        S_Pillar *new_pillar = new Pillar(element);
        new_pillar->height = coin_flip(height);
        new_pillar->right_links.resize(new_pillar->height);
        new_pillar->left_links.resize(new_pillar->height);

        S_Pillar *cur = head;
        int level = height - 1;

        while(level >= 0) {
          if(cur != head && ((Pillar *)cur)->data.first == element.first) {
            // Found a pillar that already has the key we want to insert
            delete new_pillar;
            return std::make_pair(Iterator(cur), false);
          }
          if(cur->right_links[level] == head || ((Pillar *)cur->right_links[level])->data.first > element.first) {
            // Go 1 level down
            if((unsigned int)level < new_pillar->height) {
              // Set up new_pillar's links at the current level
              new_pillar->left_links[level] = cur;
              new_pillar->right_links[level] = cur->right_links[level];
            }
            level--;
          } else {
            // Go right
            cur = cur->right_links[level];
          }
        }
        // didn't find it so we can insert

        // Attach pointers to new_pillar
        for(std::size_t i = 0; i < new_pillar->height; i++) {
          new_pillar->left_links[i]->right_links[i] = new_pillar;
          new_pillar->right_links[i]->left_links[i] = new_pillar;
        }
        num_elements++;
        return std::make_pair(Iterator(new_pillar), true);
      }

      void erase(Iterator pos) {
        Pillar *cur = ((Pillar*)pos.data);
        for(std::size_t i = 0; i < cur->height; i++) {
          cur->left_links[i]->right_links[i] = cur->right_links[i];
          cur->right_links[i]->left_links[i] = cur->left_links[i];
        }
        delete cur;
        num_elements--;
      }

      void erase(const Key_T &target) {
        erase(find(target));
      }

      void print() {
        std::vector<std::vector<Mapped_T>> matrix;

        for(S_Pillar *p = head->right_links[0]; p != head; p = p->right_links[0]) {
          std::vector<Mapped_T> value_vec;
          for(std::size_t i = 0; i < p->height; i++) {
            value_vec.push_back(((Pillar *)p)->data.second);
          }
          matrix.push_back(value_vec);
        }
        for(std::size_t i = height - 1; height > i; i--) {
          std::cout << "H";
          for(std::size_t j = 0; j < matrix.size(); j++) {
            std::cout << "\t";
            if(i < matrix[j].size()) {
              std::cout << matrix[j][i];
            }
          }
          std::cout << "\tH" << std::endl;
        }
      }

      int coin_flip(int max) {
        int ret = 1;
        while(rand() % 2 == 0) ret++;
        if(ret > max) ret = max;
        return ret;
      }


      
      /* NESTED CLASSES */

      struct S_Pillar {
        ~S_Pillar() {
        }
        std::size_t height;
        std::vector<S_Pillar *> right_links;
        std::vector<S_Pillar *> left_links;
      };

      struct Pillar : public S_Pillar {
        ValueType data;

        Pillar() : S_Pillar() {}
        Pillar(Key_T key, Mapped_T value) : data{key, value} {}
        Pillar(ValueType data) : data{data} {}
      };

      struct Iterator {
        S_Pillar *data;

        Iterator() = delete;
        Iterator(S_Pillar *seed) { this->data = seed; }
        ValueType value() { return ((Pillar *)data)->data; }

        // TODO Remove
        bool operator==(const Iterator &that) {
          return ((Pillar *)this->data) == that.data;
        }
        // TODO Remove
        bool operator!=(const Iterator &that) {
          return !(((Pillar *)this->data) == that.data);
        }

        Iterator& operator++() {
          data = data->right_links[0];
          return *this;
        };

        Iterator& operator++(int) {
          Iterator *tmp = this;
          operator++();
          return *tmp;
        }

        Iterator& operator--() {
          data = data->left_links[0];
          return *this;
        }

        Iterator& operator--(int) {
          Iterator *tmp = this;
          operator--();
          return *tmp;
        }

        ValueType &operator*() const {
          return ((Pillar *)data)->data;
        }

        ValueType *operator->() const {
          return &(((Pillar *)data)->data);
        }
      };
  };
}
#endif
