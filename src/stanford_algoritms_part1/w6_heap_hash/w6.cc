
#include <cassert>

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
};

// Похоже для алгоритма Дектсры лучше bst 

namespace fp {
template <class OP1, class OP2>
class compose_f_gx_t 
  : public std::unary_function<typename OP1::argument_type, typename OP2::result_type>
{
  OP1 op1;
  OP2 op2;
public:
  compose_f_gx_t(const OP1& o1, const OP2& o2) : op1(o1), op2(o2) {}
  typename OP1::result_type
  operator()(const typename OP2::argument_type& x) const {
    return op1(op2(x));
  }  
};

template <class OP1, class OP2>
compose_f_gx(const OP1& o1, const OP2& o2) {
  return compose_f_gx_t<OP1, OP2>(o1, o2);
}

}

bool _2sum_naive(const vector<int>& in, const int target_sum)
{
  bool finded = false;
  for (vector<int>::const_iterator at = in.begin(), end = in.end(); at != end; ++at) {
    vector<int>::const_iterator finded_it = 
      find_if(
	in.begin(), in.end(), 
	fp::compose_f_gx(bind2nd(equal_to<int>(), target_sum), bind2nd(plus<int>(), *at)));
    
    if (finded_it != in.end()) {
      finded = true;
      break;
    }
  }
  return finded;
}

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
  vector<int> in(array, array+5);
  int target_sum = 15;
  bool finded = _2sum_naive(in, target_sum);
  assert(finded == true);
  
  
  /// Q2
  // похоже можно пользоваться реализацией кучи из std::
  
}