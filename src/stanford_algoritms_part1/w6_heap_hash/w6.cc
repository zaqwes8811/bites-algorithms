
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

int main() {
  int array[] = {10,20,30,5,15};
  vector<int> v(array, array+5);
  
  // 
  for_each(begin(v), end(v), [] (const int& elem) { cout << elem << " "; });
  cout << endl;
  
  // http://stackoverflow.com/questions/7681779/easy-way-to-maintain-a-min-heap-with-stl
  make_heap(begin(v), end(v), greater<int>());  // min heap
  cout << v.front() << endl;  // TODO: top() - это первый элемент?
  
  for_each(begin(v), end(v), [] (const int& elem) { cout << elem << " "; });
  cout << endl;
  
  // http://stackoverflow.com/questions/14016921/comparator-for-min-heap-in-c
  // Где вершина то?
  // Похоже нужно использовать один и тот же компаратор
  
  
  
}