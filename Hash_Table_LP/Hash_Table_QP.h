// Student ID: 20274909

#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <iostream>
#include <list>
#include <iterator>
#include "Hash_Table_LP.h"

using namespace std;

#ifndef Hash_Table_QP_h
#define Hash_Table_QP_h

// Extern - must be defined by client
template <typename T> size_t Hash(const T& item);

template <typename T>
class Hash_Table_QP : public Hash_Table_LP<T> {
public:
   Hash_Table_QP(size_t n = Hash_Table_LP<T>::DEFAULT_INIT_CAPACITY) : Hash_Table_LP<T>(n) {
      this->_max_load_factor = _get_biggest_allowed_max_load_factor();
   }

protected:
   virtual float _get_biggest_allowed_max_load_factor() const {
      return 0.49;
   }

   virtual size_t _find_pos(const T& item) const {
      if (Hash_Table_LP<T>::_num_non_vacant_cells >= Hash_Table_LP<T>::_elems.size() 
         || Hash_Table_LP<T>::_elems.size() <= 0)
         return string::npos;

      size_t index = Hash_Table_LP<T>::_get_hash_modulus(item);
      size_t elems_size = Hash_Table_LP<T>::_elems.size();

      int odd_num = 1;

      while (Hash_Table_LP<T>::_elems[index % elems_size]._state != Hash_Table_LP<T>::Entry::VACANT) {
         if (Hash_Table_LP<T>::_elems[index % elems_size]._data == item)
            break;
         index += odd_num;
         odd_num += 2;
      }

      return (index % elems_size);
   }

   virtual void _grow_capacity() {
      if (Hash_Table_LP<T>::_elems.size() <= 0)
         return;

      size_t original_size = Hash_Table_LP<T>::_elems.size();
      size_t new_size = _next_prime(Hash_Table_LP<T>::_elems.size() * 2);

      for (size_t i = original_size; i < new_size; i++)
         Hash_Table_LP<T>::_elems.push_back(typename Hash_Table_LP<T>::Entry());
   }

   // Private helper
   static size_t _next_prime(size_t n) {
      if (n < 0) {
         return string::npos;
      }
      else if (n < 2) {
         return 2;
      }
      n++;
      while (true) {
         if (n <= 5) {
            if (n % 3 == 0)
               return n;
            if (n % 5 == 0)
               return n;
         }
         if (n % 2 == 0) {
            n++;
            continue;
         }
         if (n % 3 == 0) {
            n++;
            continue;
         }
         double _sqrt = sqrt(n);
         if (_sqrt - floor(_sqrt) == 0) {
            n++;
            continue;
         }
         int _sqrt_ceiling = ceil(_sqrt);
         int _sixth_sqrt = _sqrt_ceiling / 6;
         bool is_prime = true;
         for (int i = 1; i <= _sixth_sqrt; i++) {
            if (n % (6 * i + 1) == 0) {
               n++;
               is_prime = false;
               break;
            }
            if (n % (6 * i - 1) == 0) {
               n++;
               is_prime = false;
               break;
            }
         }
         if (is_prime)
            break;
      }
      return n;
   }

   // Don't modify below
   friend class Tests;
};

#endif