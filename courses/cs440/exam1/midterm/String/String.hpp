#ifndef CS540_STRING_HPP
#define CS540_STRING_HPP

#include <string>
#include <cstring>
#include <iostream>

class String {
  public:
    std::string value;
    String(const char* seed) : value(seed) {};
    String(const String &seed) : value(seed.value) {};
    String(const std::string &seed) : value(seed) {};

    String &operator=(const String &other) {
      if (&other == this) {
        return *this;
      }
      value = other.value;
      return *this;
    }

    String &operator=(const char *other) {
      value = std::string(other);
      return *this;
    }

    String operator+(const char *rhs) const {
      return String(value + std::string(rhs));
    }

    friend std::ostream &operator<<(std::ostream &output, const String &s) { 
      output << s.value;
      return output;            
    }
};

#endif
