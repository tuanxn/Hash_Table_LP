// Student ID: 20274909

#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <list>
#include <iterator>
#include "Hash_Table_QP.h"

using namespace std;

template<> size_t Hash<string>(const string& s) {
   return hash<string>{}(s);
}

int main()
{
   Hash_Table_QP<string> test2 = Hash_Table_QP<string>();

   //cout << hash<string>{}("a") << endl;
   //test2.insert("a");
   //test2.remove("a");
   //cout << hash<string>{}("b") << endl;
   //test2.insert("b");
   //cout << hash<string>{}("b") << endl;
   //test2.insert("c");

   //cout << test2.contains("c") << endl;
   //cout << test2.contains("C") << endl;

   //cout << test2.insert("c") << endl;

   //test2.remove("z");
   //test2.remove("b");

   //test2.insert("z");

   //test2.insert("b");

}