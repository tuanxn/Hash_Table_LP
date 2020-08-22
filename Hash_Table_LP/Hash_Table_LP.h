// Student ID: 20274909

#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <iostream>
#include <list>
#include <iterator>

using namespace std;

#ifndef Hash_Table_LP_h
#define Hash_Table_LP_h

// Extern - must be defined by client
template <typename T> size_t Hash(const T& item);

template <typename T>
class Hash_Table_LP {
protected:
   struct Entry {
      T _data; // payload
      enum STATE { ACTIVE, VACANT, DELETED } _state;
      Entry(const T& d = T(), STATE st = VACANT) : _data(d), _state(st) {}
   };

   static const size_t DEFAULT_INIT_CAPACITY = 3; // first odd prime. (Don't change)
   vector<Entry> _elems;
   size_t _size;                 // doesn't count deleted items
   size_t _num_non_vacant_cells; // does count deleted items
   float _max_load_factor;
   
   virtual size_t _get_hash_modulus(const T& item) const { // uses Hash(item), ext.
      return Hash<T>(item) % _elems.size();
   }

   virtual void _rehash() {
      // copy _elems into temporary duplicate
      vector<Entry> temp;
      for (size_t i = 0; i < _elems.size(); i++) {
         if (_elems[i]._state == Entry::ACTIVE)
            temp.push_back(Entry(_elems[i]));
         // reset elements in original vector
         // YOU NEED TO LEAVE THE DATA THE SAME
         //_elems[i]._data = T(); 
         _elems[i]._state = Entry::VACANT;
      }
      // double original size and reset members
      _grow_capacity();
      _size = 0;
      _num_non_vacant_cells = 0;
      // TODO insert into original doubled vector
      for (size_t j = 0; j < temp.size(); j++) {
         if (temp[j]._state == Entry::ACTIVE)
            insert(temp[j]._data);
      }

   }

   // Most common overrides
   virtual bool set_max_load_factor(float x) {
      if (0 < x && x <= _get_biggest_allowed_max_load_factor()) {
         _max_load_factor = x;
         return true;
      }
      return false;
   }

   virtual float _get_biggest_allowed_max_load_factor() const {
      return 0.75;
   }

   virtual size_t _find_pos(const T& item) const {
      if (_num_non_vacant_cells >= _elems.size() || _elems.size() <= 0)
         return string::npos;

      size_t index = _get_hash_modulus(item);
      size_t elems_size = _elems.size();

      while (_elems[index % elems_size]._state != Entry::VACANT) {
         if (_elems[index % elems_size]._data == item)
            break;
         index++;
      }

      return (index % elems_size);
   }

   virtual void _grow_capacity() {
      if (_elems.size() <= 0) 
         return;
     
      size_t original_size = _elems.size();
      size_t new_size = _elems.size() * 2;

      for (size_t i = original_size; i < new_size; i++)
         _elems.push_back(Entry());
   }

public:
   Hash_Table_LP(size_t n = DEFAULT_INIT_CAPACITY) {
      if (n > 0) {
         _elems.resize(n);
      }
      else {
         _elems.resize(DEFAULT_INIT_CAPACITY);
      }
      _size = 0;
      _num_non_vacant_cells = 0;
      _max_load_factor = _get_biggest_allowed_max_load_factor();
   }

   size_t get_size() const { return _size; }

   bool is_empty() const { return _size == 0; }

   bool contains(const T& item) const {
      try {
         size_t index = _find_pos(item);
         if (index == string::npos) {
            throw Table_full_exception();
         }
         else if (_elems[index]._state == Entry::VACANT) {
            throw Not_found_exception();
         }
         return true;
      }
      catch (Table_full_exception e) {
         cout << e.to_string() << endl;
         return false;
      }
      catch (Not_found_exception e) {
         cout << e.to_string() << endl;
         return false;
      }
   }

   T& find(const T& item) {
      size_t index = _find_pos(item);
      if (index == string::npos) {
         throw Table_full_exception();
      }
      else if (_elems[index]._state == Entry::VACANT) {
         throw Not_found_exception();
      }

      return _elems[index]._data;
   }

   bool clear() {
      if (is_empty())
         return false;

      for (size_t i = 0; i < _elems.size(); i++) {
         _elems[i]._state = Entry::VACANT;
      }
      return true;
   }

   bool insert(const T& item) {
      size_t index = _find_pos(item);

      if (index == string::npos) {
         return false;
      }
      else if (_elems[index]._state == Entry::VACANT) {
         _elems[index]._data = item;
         _elems[index]._state = Entry::ACTIVE;
         _size++;
         _num_non_vacant_cells++;
      }
      else if (_elems[index]._state == Entry::ACTIVE) {
         return false;
      }
      else if (_elems[index]._state == Entry::DELETED) {
         _elems[index]._state = Entry::ACTIVE;
         _size++;
      }

      if (_num_non_vacant_cells > _elems.size() * _max_load_factor)
         _rehash();

      return true;
   }

   bool remove(const T& item) {
      size_t index = _find_pos(item);

      if (index == string::npos) {
         return false;
      }
      else if (_elems[index]._state == Entry::VACANT) {
         return false;
      }
      else if (_elems[index]._state == Entry::DELETED) {
         return false;
      }
      else if (_elems[index]._state == Entry::ACTIVE) {
         _elems[index]._state = Entry::DELETED;
         _size--;
      }

      return true;
   }

   class Table_full_exception : public exception {
      public: const string to_string() const throw() { return string("Table full exception"); }
   };
   class Not_found_exception :public exception {
      public: const string to_string() const throw() { return string("Not found exception"); }
   };

   friend class Tests;
};

#endif