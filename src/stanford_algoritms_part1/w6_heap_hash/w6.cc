#include <gtest/gtest.h>

#include <cassert>

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

// C++11
#include <unordered_map>

// no standart now
//#include <ext/hash_set>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

// DANGER: Кажется с пользовательскими типами как-то не очень. Что то похоже и про строки слышал, кажется.
#include <google/dense_hash_map>

// 3rdpary
#include <boost/foreach.hpp>

// App
#include "details/io_details.h"
#include "details/fp_details.h"

using namespace std;

using std::hash;  // C++11
//using boost::hash;  // нужно точечно указать

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

int q1(const vector<int>& in) {
  // http://publib.boulder.ibm.com/infocenter/comphelp/v9v111/index.jsp?topic=/com.ibm.xlcpp9.aix.doc/standlib/stl_unordered_set.htm
  boost::unordered_set<int> htbl;
  int low = 2500;
  int high = 4000;

  // de-duplication
  BOOST_FOREACH(int val, in) {
    if (htbl.end() == htbl.find(val))
      if (val < high+1)  // можно ужать еще
	htbl.insert(val); 
  };
  assert(htbl.size() < in.size());
  assert(!htbl.empty());

  int count_unique = 0;
  for (int t = low; t < high+1; ++t) {
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

long q2_nth(const vector<int>& in) {
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
  return mean;
}

TEST(W5_6, PQuestions) {
  /// Q1
  // Задание не понятно
  //int finded = _2sum_naive(in, target);
  //assert(finded > 0);
  // TODO: сделать все три варианта
  vector<int> in = io_details::extract_records("./input_data/HashInt.txt");
  int count_unique = q1(in);  
  assert(count_unique > 0 && count_unique < 1501);
  assert(count_unique == 1477);
  assert(!in.empty());

  /// Q2
  in = io_details::extract_records("./input_data/Median.txt");
  // TODO: heap and bfs based impl.
  assert(q2_nth(in) % 10000 == 1213); 
}

namespace {
struct TaskId {
  TaskId() : v(0), w(0) {}
  TaskId(int _v, int _w) : v(_v), w(_w) {}
  
  int v;
  int w;
};

struct KeyHash {
 std::size_t operator()(const TaskId& k) const
 {
   // Watch "Eff. Java."  
   // Проблема в том как скомбинировать.
   return boost::hash<int>()(k.v) ^ (boost::hash<int>()(k.w) << 1);
 }
};
 
struct KeyEqual {
 bool operator()(const TaskId& lhs, const TaskId& rhs) const
 {
    return lhs.v == rhs.v && lhs.w == rhs.w;
 }
};

}

/// HashTables
// TODO: а есть ли адаптивные хэш-таблицы?
//
// Используется рандомизация, но как потом искать?
//
// см. effective Java - там есть про хэш коды - если переорп. equal then переорп. hashCode!
// Равные объекты должны иметь равные хэши.
//
// DANGER: Good hashtable:
// - good hash functon - равномерно разбрасывает по бакетам
//   && good load factor - n/(count_buckets) - при первом условии делает связанные списки (или аналог) как можно короче
//   && O(1) calc hash value
// 
// Pro:
// 
//
// Cons:
//   - для больших объемов данных?
//   - O(1) при вычислении это да, но константы могут быть большими
//   - нельзя делать сложные выборки
//
// Java:
//  http://docs.oracle.com/javase/7/docs/api/java/util/HashMap.html
//
// C++:
// TODO: как переопределение хэш функции влияет на работу? Может быть использовать двумерную таблицу?
//   Похоже по получнному ключу таблица еще раз считает хэш.
//
// Benchmarks:
//   http://research.neustar.biz/tag/unordered_map/
//   http://preshing.com/20110603/hash-table-performance-tests/
//
// Hash functions:
//   http://programmers.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed
//
TEST(DataStructures, HashTables) {
  // http://stackoverflow.com/questions/2179946/i-would-like-to-see-a-hash-map-example-in-c
  //
  // http://msdn.microsoft.com/en-us/library/1s1byw77.aspx
  // Если уточнять ключ, то как быть с коллизиями - в задаче при поиске нужно быть уверенным.
  // Хотя... Вообще два разных объекта с равными кешами очень вероятны. Еще передается функция эквивалетности.
  //
  //
  //unordered_map<TaskId, int> table;  // not compiled - можно, но нужно уточнить операции с ключами
  unordered_map<int, int> table;
  
  boost::unordered_map<TaskId, int, KeyHash, KeyEqual> htbl;
  google::dense_hash_map<TaskId, int, KeyHash, KeyEqual> g_tbl;
  g_tbl.set_empty_key(TaskId(0, 0));
  g_tbl[TaskId(1, 5)] = 9;
  
  cout << g_tbl[TaskId(1, 5)] << endl;
  assert(g_tbl.end() != g_tbl.find(TaskId(1, 5)));
  
}

TEST(DataStructures, BloomFilter) {
  // Bloom filter:
  //   http://code.google.com/p/bloom/
  //
  // Pro:
  //   - more then hashtable space efficient - DANGER: не всегда можно проверить ложноположительное срабатывание
  // Cons:
  //   - can't store value
  //   - can't delete
  //   - ложно положительные срабатывания
}

// Хранение в разнобой
// http://preshing.com/20130107/this-hash-table-is-faster-than-a-judy-array/
// TODO: See sparese arrays, skeep list
TEST(DataStructures, JudyArrays) {
  // http://judy.sourceforge.net/
  // Cons:
  //   - похоже запатентовано
}


/// BST
// Sorted array 
//   search - log(n)
//   selection(orger) - 1
//   min/max - 1
//   predcessor/successor - 1
//   rank(key) - сколько меньше ключа - log(n)
//   << - n
//  TROUBLE: insert, deletions
//
// -> BST