#include <gtest/gtest.h>

// C++98
#include <vector> 
#include <ostream>
#include <string>

// C++11
#include <unordered_map>

// 3rdparty
#include <boost/unordered_map.hpp>
#include <google/dense_hash_map>
#include <google/sparse_hash_map>

// inner
#include "visuality/view.h"
#include "details/io_details.h"

using namespace std;
using view::operator<<;
using io_details::Item;
using io_details::get_test_items;
using io_details::get_dyn_items;

namespace {
struct TaskId {
  TaskId(int _w_bound, int _idx) : w_bound(_w_bound), idx(_idx) {}
  TaskId() : w_bound(0), idx(0) {}
  int w_bound;
  int idx;
};

struct KeyHash {
  std::size_t operator()(const TaskId& k) const
  {
    // Watch "Eff. Java."  
    // Проблема в том как скомбинировать.
    // TODO: other method calc.
    //
    // Good hash for pair:
    //  http://stackoverflow.com/questions/12764645/good-hash-function-with-2-integer-for-a-special-key
    //  http://stackoverflow.com/questions/2634690/good-hash-function-for-a-2d-index
    //  !!! http://web.archive.org/web/20071223173210/http://www.concentric.net/~Ttwang/tech/inthash.htm
    //
    //return boost::hash<pair<int, int> >()(make_pair(k.idx,k.w_bound));  // slow
    return boost::hash<int>()(k.idx) ^ (boost::hash<int>()(k.w_bound) << 1);  // max speed
    //return boost::hash<int>()(k.w_bound) ^ (boost::hash<int>()(k.idx) << 1);
    //return boost::hash<int>()(k.idx) ^ (boost::hash<int>()(k.w_bound) >> 1);
    //return (size_t)((k.idx << 19) | (k.w_bound << 7));
    //return boost::hash<int>()(k.idx) * 37 + (boost::hash<int>()(k.w_bound));
    //return ((997 + boost::hash<int>()(k.idx)))*997 + boost::hash<int>()(k.w_bound);
  }
};

struct KeyHashStd {
  std::size_t operator()(const TaskId& k) const
  {
    // Watch "Eff. Java."  
    // Проблема в том как скомбинировать.
    // TODO: other method calc.
    // Влияет очень не слабо! Возможно лучше 2D version
    return ((std::hash<int>()(k.idx)) ^ (std::hash<int>()(k.w_bound) << 1));
    //return boost::hash<int>()(k.idx) * 37 + (boost::hash<int>()(k.w_bound));
    return ((51 + std::hash<int>()(k.idx)))*51 + std::hash<int>()(k.w_bound);
  }
};
 
struct KeyEqual {
  bool operator()(const TaskId& lhs, const TaskId& rhs) const
  {
    return lhs.idx == rhs.idx && lhs.w_bound == rhs.w_bound;
  }
};

ostream& operator<<(ostream& o, const TaskId& id) 
{
  o << "(" << id.w_bound << ", " << id.idx << ")\n";
  return o;
}
    
// Returns the maximum value that can be put in a knapsack of capacity W
int knapSackExp(int w_bound, int n, const vector<Item>& items)
{
  // Base Case
  if (n == 0 || w_bound == 0)
      return 0;

  // If weight of the nth item is more than Knapsack capacity W, then
  // this item cannot be included in the optimal solution
  cout << w_bound << " "<< n << endl;
  if (items[n-1].w > w_bound)
      return knapSackExp(w_bound, n-1, items);

  // Return the maximum of two cases: (1) nth item included (2) not included
  else {
    int sum_values = std::max( 
      knapSackExp(w_bound - items[n-1].w, n-1, items) + items[n-1].v, 
      knapSackExp(w_bound,                n-1, items));

    return sum_values;
  }   
}

// Returns the maximum value that can be put in a knapsack of capacity W
template <typename Store>
int knapSack_hashtable(const TaskId& id, 
                       const vector<Item>& items, 
                       Store& store)
{
  // check task is solved
  if (store.end() != store.find(id)) {
    return store[id];
  }
  
  // Work
  int n = id.idx;
  int w_bound = id.w_bound;
  
  // Base Case
  if (n == 0 || w_bound == 0)
      return 0;

  // If weight of the nth item is more than Knapsack capacity W, then
  // this item cannot be included in the optimal solution
  if (items[n-1].w > w_bound) {
    return 
      knapSack_hashtable(TaskId(w_bound,                n-1), items, store);
  } else {
    // Return the maximum of two cases: (1) nth item included (2) not included
    int sum_values = std::max( 
      knapSack_hashtable(TaskId(w_bound - items[n-1].w, n-1), items, store) + items[n-1].v, 
      knapSack_hashtable(TaskId(w_bound,                n-1), items, store));
    
    // Добавляем решенную
    store.insert(make_pair(id, sum_values));
    return sum_values;
  } 
}

//template <typename Store>
// Too slooow
int knapSack_hashtable_2d(const TaskId& id, 
                         const vector<Item>& items, 
                         std::unordered_map<int, std::unordered_map<int, int> >& store)
{
  // check task is solved
  {
    std::unordered_map<int, std::unordered_map<int, int> >::iterator it = store.find(id.w_bound);
    if (store.end() != it) {
      std::unordered_map<int, int> tmp = it->second;
      if (tmp.end() != tmp.find(id.idx))
        return tmp[id.idx];
    }
  }
  
  // Work
  int n = id.idx;
  int w_bound = id.w_bound;
  
  // Base Case
  if (n == 0 || w_bound == 0)
      return 0;

  // If weight of the nth item is more than Knapsack capacity W, then
  // this item cannot be included in the optimal solution
  if (items[n-1].w > w_bound) {
    return 
      knapSack_hashtable_2d(TaskId(w_bound,                n-1), items, store);
  } else {
    // Return the maximum of two cases: (1) nth item included (2) not included
    int sum_values = std::max( 
      knapSack_hashtable_2d(TaskId(w_bound - items[n-1].w, n-1), items, store) + items[n-1].v, 
      knapSack_hashtable_2d(TaskId(w_bound,                n-1), items, store));
    
    // Добавляем решенную
    // Если первый раз, то создаем пустую таблицу
    if (store.end() == store.find(id.w_bound)) {
      store.insert(make_pair(id.w_bound, std::unordered_map<int, int>()));
    }
    
    store[id.w_bound].insert(make_pair(id.idx, sum_values));
      
    //store.insert(make_pair(id, sum_values));
    return sum_values;
  } 
}

int knapSack(int W, int wt[], int val[], int n)
{
   // Base Case
   if (n == 0 || W == 0)
       return 0;
 
   // If weight of the nth item is more than Knapsack capacity W, then
   // this item cannot be included in the optimal solution
   //cout << W << " "<< n << endl;
   if (wt[n-1] > W)
       return knapSack(W, wt, val, n-1);
 
   // Return the maximum of two cases: (1) nth item included (2) not included
   else 
     return std::max( 
      knapSack(W - wt[n-1], wt, val, n-1) + val[n-1],
      knapSack(W,           wt, val, n-1));
}
}

TEST(W3, Wis) 
{
  int arr[] = {0, 1, 4, 5, 4};  // 0 - guard
  vector<int> W(arr, arr + sizeof(arr)/sizeof(arr[0]));
  cout << W;

  // Work
  vector<int> A(W.size(), 0);
  A[1] = W[1]; 
  for (size_t i = 2; i < W.size(); ++i) {
    // верширу либо добавляем либо
    A[i] = std::max(A[i-1], A[i-2] + W[i]);
    cout << A;
  }
  
  // Reconstruction solution 
  int i = W.size() - 1;
  while (i >= 1) {
    // Рассматриваем подзадачи, но сам вес из текущего узла
    // Похоже на рассуждение при разработке алгоритма
    if (A[i-1] >= A[i-2] + W[i]) {
      // Веришина была исключена
      --i;
    } else {
      // Веришина была включена, и нужно перескочить
      cout << W[i] << " " << i << endl;
      i -= 2;
    }
  }
  
}

TEST(W3, KnapsackProblem) 
{
  pair<int, vector<Item > > tmp = get_test_items("NoFile");
  vector<Item> items = tmp.second;
  int W = tmp.first;
  
  
  // A[0,0]
  int N = items.size();
  vector<vector<int> > A(N, vector<int>(W+1, 0)); 
  for (int i = 1; i < N; ++i) {
    
    // Значения объема может быть любым с точностью до единицы
    for (int x = 0; x <= W; ++x) {
      int w_boundary = x - items[i].w;
      int v = items[i].v;
      if (w_boundary < 0) {
        w_boundary = x;
        v = 0;
      }
      
      // Read
      int invalid = A[i-1][x];
      int valid = A[i-1][w_boundary];
      
      // Write
      A[i][x] = std::max(invalid, valid + v);
    }
  }
  
  cout << " " << A;

  // Reconstruction
}

TEST(W3, BigKnapsackProblem) 
{
  // V(i, x) = max(V(i-1, x), v(i) + V(i-1, x-w(i))
  // if w(i) > x 
  //   V(i, x) = V(i-1, x)
  //
  //
  // Очень большой объем данных
  // - Рекурсивная реализация
  // - Использовать только 2 колонки - скорости это врядли прибавит
  // - Образать веса которых не может быть - как в задаче из 1 части - похоже в алгоритме уже есть фильтр
  // - Хэш таблица похоже безсмысленна, она реализована на основе массива - хотя...
  // - Сортировка
  pair<int, vector<Item > > tmp = get_test_items("NoFile");
  vector<Item> items = tmp.second;
  int W = tmp.first;
  
  
  // A[0,0]
  int N = items.size();
  vector<vector<int> > A(N, vector<int>(W+1, 0)); 
  for (int i = 1; i < N; ++i) {
    const int v = items[i].v;
    const int w = items[i].w;
    
    // Значения объема может быть любым с точностью до единицы
    // Итерация по емкостям
    for (int x = 0; x <= W; ++x) {
      int w_boundary = x - w;
      
      if (w_boundary < 0) {
        A[i][x] = A[i-1][x];  // просто копирование
      } else {
        A[i][x] = std::max(A[i][x], A[i-1][w_boundary] + v);
      }
    }
  }
  
  cout << " " << A;
}

TEST(W3, GeeksForGeek) 
{ 
  pair<int, vector<Item > > tmp = get_test_items("NoFile");
  vector<Item> items = tmp.second;
  int W = tmp.first;

  int count = items.size();
  printf("sum(v(i)) = %d \n", knapSackExp(W, count, items));
}

TEST(W3, GeeksForGeekSrc) {
    int val[] = {60, 100, 120};
    int wt[] = {10, 20, 30};
    int  W = 50;
    int n = sizeof(val)/sizeof(val[0]);
    printf("%d", knapSack(W, wt, val, n));
}

TEST(W3, GeeksForGeek_hashtable) 
{ 
  pair<int, vector<Item > > tmp = get_test_items("NoFile");
  vector<Item> items = tmp.second;
  int W = tmp.first;

  int count = items.size();
  TaskId root(W, count);
  boost::unordered_map<TaskId, int, KeyHash, KeyEqual> store;
  int result = knapSack_hashtable(root, items, store);
  printf("sum(v(i)) = %d \n", result);
  assert(result == 8);
}

TEST(W3, GeeksForGeek_hashtable_homework) 
{ 
  //pair<int, vector<Item > > tmp = get_dyn_items("./input_data/knapsack1.txt");
  pair<int, vector<Item > > tmp = get_dyn_items("./input_data/knapsack_big.txt");
  
  ///*
  vector<Item> items = tmp.second;
  int W = tmp.first;

  int count = items.size();
  TaskId root(W, count);
  
  boost::unordered_map<TaskId, int, KeyHash, KeyEqual> store;//(4000);
  //std::unordered_map<TaskId, int, KeyHashStd, KeyEqual> store;
  
  
  /* 
  //DANGER: it's slooooow or incorrect
  //google::dense_hash_map<
  google::sparse_hash_map<
    TaskId, int
    , KeyHashStd, KeyEqual
    > store;
  //store.set_empty_key(TaskId());
  //*/
  cout << store.max_bucket_count() << endl;
  
  int result = knapSack_hashtable(root, items, store);
  printf("sum(v(i)) = %d \n", result);
  assert(result == 4243395);
}

// Very slooow.
TEST(W3, GeeksForGeek_hashtable_2homework) 
{ 
  pair<int, vector<Item > > tmp = get_dyn_items("./input_data/knapsack1.txt");
  //pair<int, vector<Item > > tmp = get_test_items("./input_data/knapsack_big.txt");
  
  ///*
  vector<Item> items = tmp.second;
  int W = tmp.first;

  int count = items.size();
  TaskId root(W, count);
  //boost::unordered_map<TaskId, int, KeyHash, KeyEqual> store;
  std::unordered_map<int, std::unordered_map<int, int> > store;
  
  int result = knapSack_hashtable_2d(root, items, store);
  printf("sum(v(i)) = %d \n", result);
  //assert(result == 4243395);
}



// 
TEST(W3, Genome) 
{
  
}

TEST(W3, OptBST) 
{
  // Скорее всего тольков в теории
}