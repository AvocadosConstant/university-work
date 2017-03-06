#ifndef _SKIPLIST_HPP_
#define _SKIPLIST_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

namespace cs540 { template <typename Key_T, typename Mapped_T> class SkipList {
    typedef std::pair<Key_T, Mapped_T> ValueType;

    struct S_Pillar{
      std::size_t height;
      std::vector<S_Pillar *> right_links;
      std::vector<S_Pillar *> left_links;
    };

    struct Pillar : public S_Pillar{
      ValueType data;

      Pillar() : S_Pillar() {}
      Pillar(Key_T key, Mapped_T value) : data{key, value} {}
      Pillar(ValueType data) : data{data} {}
    };

    public:
      S_Pillar *head;
      std::size_t height;
      std::size_t num_elements;
      static const std::size_t DEFAULT_HEIGHT = 16;

      SkipList() {
        num_elements = 0;
        head = new S_Pillar();
        height = DEFAULT_HEIGHT;
        for(std::size_t i = 0; i < height; i++) {
          head->right_links.push_back(head);
          head->left_links.push_back(head);
        }
        std::cout << "Constructed" << std::endl;
      }

      // TODO Replace return type with std::pair<Iterator, bool> 
      void insert(const ValueType &element) {

        Pillar *tmp = new Pillar(element);
        tmp->height = 1 + (rand() % 16);
        std::cout << "Inserting " << tmp->data.first << ", " << tmp->data.second << std::endl;

        for(std::size_t i = 0; i < tmp->height; i++) {
          tmp->right_links.push_back(head);
          head->left_links[i]->right_links[i] = tmp;
          tmp->left_links.push_back(head->left_links[i]);
          head->left_links[i] = tmp;
        }
        /*
        std::cout << "tmp has " << tmp->right_links.size() << " right links" << std::endl;
        std::cout << "tmp has " << tmp->left_links.size() << " left links" << std::endl;
        if(tmp->left_links[0] != head) {
          std::cout << "tmp prev is " << ((Pillar *)tmp->left_links[0])->data.second << std::endl;
        }
        if(tmp->right_links[0] != head) {
          std::cout << "tmp next is " << ((Pillar *)tmp->right_links[0])->data.second << std::endl;
        }
        */
        num_elements++;
      }

      /*
      Iterator find(const Key_T &);

      ConstIterator find(const Key_T &) const;

      Mapped_T &at(const Key_T &) const;

      const Mapped_T &at(const Key_T &) const;
      */

      void print() {
        std::vector<std::vector<Mapped_T>> matrix;

        //for(std::size_t i = 0; i < num_elements; i++) {
        for(S_Pillar *p = head->right_links[0]; p != head; p = p->right_links[0]) {
          std::vector<Mapped_T> value_vec;
          for(std::size_t i = 0; i < p->height; i++) {
            value_vec.push_back(((Pillar *)p)->data.second);
          }
          matrix.push_back(value_vec);
          std::cout << "Pushed to matrix value_vec of size " << value_vec.size() << std::endl;
        }
        std::cout << "Matrix is " << matrix.size() << "x" << num_elements << std::endl;

        for(std::size_t i = height - 1; height > i; i--) {
          //std::cout << "Iterating through layer " << i << std::endl;
          for(std::size_t j = 0; j < matrix.size(); j++) {
            //std::cout << "Iterating through column " << j << std::endl;
            //std::cout << "matrix[j].size() = " << matrix[j].size() << std::endl;
            if(i < matrix[j].size()) {
              //std::cout << "mat[" << i << "][" << j << "] = " << matrix[j][i];
              std::cout << matrix[j][i];
            }
            std::cout << "\t";
          }
          std::cout << std::endl;
        }
      }
  };
}

#endif
