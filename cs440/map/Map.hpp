#ifndef _MAP_HPP_
#define _MAP_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <utility>
#include <vector>
#include <initializer_list>
//#include "SkipList.hpp"

namespace cs540 {
  template <typename Key_T, typename Mapped_T> class Map {
    typedef std::pair<Key_T, Mapped_T> ValueType;
    //friend class cs540::SkipList<Key_T, Mapped_T>;

    public:
      class SkipList;
      struct Iterator;
      struct ConstIterator;
      struct ReverseIterator;
      SkipList skiplist;

      /****************************************/
      /* Constructors and Assignment Operator */
      /****************************************/

      Map() {
        // TODO
        std::cout << "Default constructed" << std::endl;
      }
      Map(const Map &) {
        // TODO
        std::cout << "Copy constructed" << std::endl;
      }

      // Copy assignment operator
      Map &operator=(const Map &);

      // Initializer list constructor
      Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> elements) {
        // TODO
        std::cout << "Initializer list constructed" << std::endl;
      }

      // Destructor
      ~Map() {
        // TODO
        std::cout << "Destructed" << std::endl;
      }

      /********/
      /* Size */
      /********/

      // Returns number of elements
      std::size_t size() const;

      // Returns true if the Map has no entries in it
      bool empty() const;

      /*************/
      /* Iterators */
      /*************/

      // Returns an Iterator pointing to the first element in order.
      Iterator begin();

      // Returns an Iterator pointing one past the last element, in order.
      Iterator end();

      // Returns a ConstIterator pointing to the first element, in order.
      ConstIterator begin() const;

      // Returns a ConstIterator pointing one past the last element, in order.
      ConstIterator end() const;

      // Returns an ReverseIterator to the first element in reverse order, 
      // which is the last element in normal order.
      ReverseIterator rbegin();

      // Returns an ReverseIterator pointing to one past the last element in 
      // reverse order, which is one before the first element in normal order.
      ReverseIterator rend();

      /******************/
      /* Element Access */
      /******************/

      // Returns an iterator to the given key. If the key is not found, return 
      // the end() iterator.
      Iterator find(const Key_T &);

      // Returns an const iterator to the given key. If the key is not found, 
      // return the end() iterator.
      ConstIterator find(const Key_T &) const;

      // Returns a reference to the mapped object at the specified key. If the
      // key is not in the Map, throws std::out_of_range.
      Mapped_T &at(const Key_T &) const;

      // Returns a const reference to the mapped object at the specified key.
      // If the key is not in the map, throws std::out_of_range.
      const Mapped_T &at(const Key_T *) const;

      // If key is in the map, return a reference to the corresponding mapped 
      // object. If it is not, value initialize a mapped object for that key 
      // and returns a reference to it. This operator may not be used for a 
      // Mapped_T class type that does not support default construction.
      Mapped_T &operator[](const Key_T &);

      /*************/
      /* Modifiers */
      /*************/

      // Inserts the given pair into the map. If the key does not already exist 
      // in the map, it returns an iterator pointing to the new element, and 
      // true. If the key already exists, no insertion is performed nor is the 
      // mapped object changed, and it returns an iterator pointing to the 
      // element with the same key, and false.
      std::pair<Iterator, bool> insert(const ValueType &);

      // Inserts the given object or range of objects into the map. In the 
      // second version, the range of objects inserted includes the object 
      // range_beg points to, but not the object that range_end points to. In 
      // other words, the range is half-open. The iterator returned in the 
      // first version points to the newly inserted element. There must be only 
      // one constructor invocation per object inserted. Note that the range 
      // may be in a different container type, as long as the iterator is 
      // compatible. A compatible iterator would be one from which a ValueType 
      // can be constructed. For example, it might be from a 
      // std::vector<std::pair<Key_T, Mapped_T>>. There might be any number of 
      // compatible iterator types, therefore, the range insert is a member template.
      template <typename IT_T> void insert(IT_T range_beg, IT_T range_end);

      // Removes the given object from the map.
      void erase(Iterator pos);

      // Removes the given object from the map. Throws std::out_of_range if the
      // key is not in the Map.
      void erase(const Key_T &);

      // Removes all elements from the map.
      void clear();

      /******************/
      /* Nested Classes */
      /******************/
      struct Iterator {
        Iterator(const Iterator &) {}

        // Might not be necessary to implement
        Iterator& operator=(const Iterator &);

        Iterator& operator++();

        Iterator& operator++(int);

        Iterator& operator--();

        Iterator& operator--(int);

        ValueType &operator*() const;

        ValueType *operator->() const;
      };

      class SkipList {
        struct S_Pillar {
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
            srand(time(NULL));
          }

          Iterator find(const Key_T &);

          ConstIterator find(const Key_T &) const;

          Mapped_T &at(const Key_T &);

          const Mapped_T &at(const Key_T &) const;

          // TODO Replace return type with std::pair<Iterator, bool>
          void insert(const ValueType &element) {

            Pillar *tmp = new Pillar(element);
            tmp->height = coin_flip(height);
            //std::cout << "Inserting " << tmp->data.first << ", " << tmp->data.second << std::endl;

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

          void erase(Iterator pos);

          void erase(const Key_T &);

          void print() {
            std::vector<std::vector<Mapped_T>> matrix;

            //for(std::size_t i = 0; i < num_elements; i++) {
            for(S_Pillar *p = head->right_links[0]; p != head; p = p->right_links[0]) {
              std::vector<Mapped_T> value_vec;
              for(std::size_t i = 0; i < p->height; i++) {
                value_vec.push_back(((Pillar *)p)->data.second);
              }
              matrix.push_back(value_vec);
              //std::cout << "Pushed to matrix value_vec of size " << value_vec.size() << std::endl;
            }
            //std::cout << "Matrix is " << matrix.size() << "x" << num_elements << std::endl;

            for(std::size_t i = height - 1; height > i; i--) {
              //std::cout << "Iterating through layer " << i << std::endl;
              std::cout << "H";
              for(std::size_t j = 0; j < matrix.size(); j++) {
                //std::cout << "Iterating through column " << j << std::endl;
                //std::cout << "matrix[j].size() = " << matrix[j].size() << std::endl;

                std::cout << "\t";

                if(i < matrix[j].size()) {
                  //std::cout << "mat[" << i << "][" << j << "] = " << matrix[j][i];
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
      };
  };

  /**************/
  /* Comparison */
  /**************/

  /* TODO
  template <typename Key_T, typename Mapped_T> bool operator==(const Map &, const Map &);
  template <typename Key_T, typename Mapped_T> bool operator!=(const Map &, const Map &);
  template <typename Key_T, typename Mapped_T> bool operator<(const Map &, const Map &);
  */
}
#endif
