// https://launchpad.net/libmct

//#define GOOGLE_HASH_MAPS

#include "details/io_details.h"
#include "details/fp_details.h"

#include <gtest/gtest.h>

//#include <ext/hash_set>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#ifdef GOOGLE_HASH_MAPS
// DANGER: Кажется с пользовательскими типами как-то не очень. Что то похоже и про строки слышал, кажется.
// Exsist depend on K and V type
#  include <google/dense_hash_map>
#endif
#include <boost/foreach.hpp>

#include <cassert>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>

using namespace std;

using std::hash;  // C++11
//using boost::hash;  // нужно точечно указать

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

#ifdef GOOGLE_HASH_MAPS
  google::dense_hash_map<TaskId, int, KeyHash, KeyEqual> g_tbl;
  g_tbl.set_empty_key(TaskId(0, 0));
  g_tbl[TaskId(1, 5)] = 9;
  
  cout << g_tbl[TaskId(1, 5)] << endl;
  assert(g_tbl.end() != g_tbl.find(TaskId(1, 5)));
#endif
  
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

