#include <gtest/gtest.h>

// C++98
#include <vector> 
#include <ostream>
#include <string>

// 3rdparty
#include <boost/unordered_map.hpp>

#include "tbb/task_scheduler_init.h"
#include "tbb/task.h"
#include "tbb/concurrent_hash_map.h"
#include <tbb/scalable_allocator.h>

// inner
#include "visuality/view.h"
#include "details/io_details.h"

using namespace std;
using namespace tbb;

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

// Structure that defines hashing and comparison operations for user's type.
struct TBBHashCompare {
    static size_t hash( const TaskId& k ) {
        return boost::hash<int>()(k.idx) ^ (boost::hash<int>()(k.w_bound) << 1);//KeyHash()(k);
    }
    //! True if strings are equal
    static bool equal( const TaskId& lhs, const TaskId& rhs ) {
        return lhs.idx == rhs.idx && lhs.w_bound == rhs.w_bound;//KeyEqual()(x,y);
    }
};

// Аллокатор тоже не помогает
typedef concurrent_hash_map<TaskId, int, TBBHashCompare, tbb::scalable_allocator<pair<TaskId, int> > > TaskTable;
TaskTable g_task_table;

// DANGER: алгоритм пробуксовывает - иногда решает уже решенные задачи
template <typename Store>
class KnapsackTaskSerial : public task  {
  Store* const store_;
  const vector<Item>* items_;
  int* const r_;
  const TaskId id_;
  
  KnapsackTaskSerial(const KnapsackTaskSerial&);
  KnapsackTaskSerial& operator=(const KnapsackTaskSerial&);
  
public:
  KnapsackTaskSerial(const vector<Item>& items, Store& store, int& result, const TaskId& id) 
    : store_(&store), items_(&items), r_(&result), id_(id)
  {}
  
  task* execute()
  { 
    const bool connect = true;
    // Похоже так лихо не выйдет - задача обязуется выполнить вычисления а сумма фейковая
    // Если решена то вернуть результат
    if (connect) {
      TaskTable::const_accessor a;
      if (store_->find(a, id_)) {
        *r_ = a->second;
        return NULL;
      }
    }
    
    // Некоторые задачи отбросили, но некоторые в процессе решения и будут готовы к моменту записи
    {
      // Work
      int n = id_.idx;
      int w_bound = id_.w_bound;
      
      // Base Case
      if (n == 0 || w_bound == 0) {
        *r_ = 0;  // можно сохранять, но на производительность сильно не влияет
        return NULL;
      }
      
      // Prepare
      int delta = 0;
      TaskId work_id(w_bound, n-1);
      if (!((*items_)[n-1].w > w_bound)) {
        work_id = TaskId(w_bound-(*items_)[n-1].w, n-1);
        delta = (*items_)[n-1].v;
      }
      TaskId old_task(w_bound, n-1);

      // Actions 
      int x = 0;
      int y = 0;
      KnapsackTaskSerial& a = *new( allocate_child() ) KnapsackTaskSerial(*items_, *store_, x, work_id);
      KnapsackTaskSerial& b = *new( allocate_child() ) KnapsackTaskSerial(*items_, *store_, y, old_task);
      // Set ref_count to 'two children plus one for the wait".
      set_ref_count(3);
      // Start b running.
      spawn( b );
      // Start a running and wait for all children (a and b).
      spawn_and_wait_for_all(a);
      
      x += delta;
      *r_ = std::max(x, y);
    }
    
    if (connect) {
      // DANGER: похоже некоторые задачи таки решаются не один раз - и это уменьшает быстродействие
      TaskTable::accessor a;
      if (store_->insert(a, id_)) {
        a->second = *r_;
      } else {
        // DANGER: промахов очень много
        //cout << "BAD: Vain solved\n";
      }
    }
    
    return NULL;
  }
};
}
 
TEST(W3, ParallelKnap) {
  pair<int, vector<Item > > tmp = 
    get_dyn_items("./input_data/knapsack_big.txt");
    //get_test_items("NoFile");
  vector<Item> items = tmp.second;
  int W = tmp.first;
  int count = items.size();
  int result = 0;
  
  TaskId root(W, count);
  //int store = 0;
  KnapsackTaskSerial<TaskTable>& a = *new(task::allocate_root()) 
      KnapsackTaskSerial<TaskTable>(items, g_task_table, result, root);
  task::spawn_root_and_wait(a);
  //KnapsackTaskSerial<int>(items, store, result, root).execute();
  
  printf("sum(v(i)) = %d \n", result);
  //assert(result == 8);
  assert(result == 4243395);
}