
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

template <typename T>
class Heap {
public:
  // удаляет элемент - тут сразу проблема как со стеком
  // setter не должен возвращать по значению
  //T extract_min(); -> 
  T top() const;
  void pop();
  
  void insert(const T& val);
  
  void heapify(const vector<T>& unordered_data);
  
  // Вообще как это сделать? - нужно найти позицию
  // http://stackoverflow.com/questions/2372994/search-an-element-in-a-heap
  //void delete_elem(const T& val);  // кажется нужно для алгоритма Дейкстры
  
  // Trouble
  // http://stackoverflow.com/questions/649640/how-to-do-an-efficient-priority-update-in-stl-priority-queue
  // http://stackoverflow.com/questions/14016189/does-changing-a-priority-queue-element-result-in-resorting-the-queue
  void increase_decrease_key(const T& key);
}

// Похоже для алгоритма Дектсры лучше bst

int main() {
  int array[] = {10,20,30,5,15};
  vector<int> v(array, array+5);
  
  for_each(begin(v), end(v), [] (const int& elem) { cout << elem << " "; });
  cout << endl;
  
  // http://stackoverflow.com/questions/7681779/easy-way-to-maintain-a-min-heap-with-stl
  // по умолчанию less, и выходит max-heap
  make_heap(begin(v), end(v), greater<int>());  // min heap
  cout << v.front() << endl;  // TODO: top() - это первый элемент?
  
  for_each(begin(v), end(v), [] (const int& elem) { cout << elem << " "; });
  cout << endl;
  
  // http://stackoverflow.com/questions/14016921/comparator-for-min-heap-in-c
  // Где вершина то?
  // Похоже нужно использовать один и тот же компаратор
  
  /// Q1
  // TODO: сделать все три варианта
  
  /// Q2
  
}