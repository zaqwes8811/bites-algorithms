
#include <cassert>

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

// no standart now
//#include <ext/hash_set>
#include <boost/unordered_set.hpp>

// 3rdpary
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

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
inline compose_f_gx_t<OP1, OP2>
compose_f_gx(const OP1& o1, const OP2& o2) {
  return compose_f_gx_t<OP1, OP2>(o1, o2);
}

}

// Precond.:
//  Keys is uniq.
//
int _2sum_naive(const vector<int>& in, const int target)
{
  int count_unique = 0;
  for (vector<int>::const_iterator at = in.begin(), end = in.end(); at != end; ++at) {
    
    // С границами что-то не то
    // Ищет только первый, как и нужно
    vector<int>::const_iterator finded_it = find_if(
	at, // begin  TODO: may be bug
	//in.begin()
	in.end(), 
	fp::compose_f_gx(bind2nd(equal_to<int>(), target), bind2nd(plus<int>(), *at)));
    
    if (finded_it != in.end())
      count_unique++;
  }
  return count_unique;
}

vector<int> extract_records(const string& filename) 
{
  fstream stream(filename.c_str());
  if (!stream)
    throw runtime_error("Error: can't open file");

  vector<int> records;
  // IO operations
  { 
    records.reserve(500000);
    string line;  // и не видна, и не в цикле
    while (true) {
      // можно и в буффер читать, но так показалось что проще завершить чтение
      if (!std::getline(stream, line))  
	break;
      
      try {
	int i = 0;
	stringstream ss(line);
	//i = boost::lexical_cast<int>(line);
	ss >> i;
	records.push_back(i);
      } catch (const boost::bad_lexical_cast& e) {
	throw;
      }
    }
  }
  return records;  
}

int q1(const vector<int>& in) {
  // http://publib.boulder.ibm.com/infocenter/comphelp/v9v111/index.jsp?topic=/com.ibm.xlcpp9.aix.doc/standlib/stl_unordered_set.htm
  boost::unordered_set<int> htbl;

  // de-duplication
  BOOST_FOREACH(int val, in) {
    if (htbl.end() == htbl.find(val))
      htbl.insert(val); 
  };
  assert(htbl.size() < in.size());
  assert(!htbl.empty());

  int count_unique = 0;
  for (int t = 2500; t < 4000+1; ++t) {
    BOOST_FOREACH(int x, htbl) {
      int y = t - x;
      if (htbl.find(y) != htbl.end()) {
	if (x != y) {
	  count_unique++;
	  break;
	}
      }
    }
  }
  return count_unique;
}

int main() {
  int array[] = {10,20,30,5,15};

  /// Q1
  // Задание не понятно
  //int finded = _2sum_naive(in, target);
  //assert(finded > 0);
  // TODO: сделать все три варианта
  const vector<int> in 
    //= extract_records("../input_data/HashInt.txt");
    = extract_records("../input_data/Median.txt");
    //(array, array+5);
  //int count_unique = q1(in);  
  //assert(count_unique > 0 && count_unique < 1501);
  //assert(count_unique == 1477);
  assert(!in.empty());

  /// Q2
  // TODO: heap and bfs based impl.
  vector<int> in_stream = in;
  vector<int> cursor;
  long mean = 0;
  cursor.reserve(in.size());
  BOOST_FOREACH(int elem, in_stream) {
    cursor.push_back(elem);   
    int size = cursor.size();
    int nth = 0;
    if (size % 2 == 1) {
      nth = (size + 1) / 2 - 1;
    } else {
      nth = size / 2 - 1;
    }
    
    // Возможно частичная сортировка
    assert(cursor.begin()+nth != cursor.end());
    nth_element(cursor.begin(), cursor.begin()+nth, cursor.end());
    mean += cursor[nth];
  }
  cout << mean % 10000 << endl;
  
  /*
  // похоже можно пользоваться реализацией кучи из std::
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
  */
  
}