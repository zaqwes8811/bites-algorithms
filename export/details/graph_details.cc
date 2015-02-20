//#define USE_TBB

#include "graph_details.h"

//#include <tbb/tbb.h>
//#include <tbb/parallel_for.h>

#include <boost/foreach.hpp>

#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <set>
#include <stdexcept>
#include <algorithm>

#define for_each_ BOOST_FOREACH

using namespace std;
//using namespace tbb;
using io_details::Arrow;

namespace graph_details {
 
Neighbor::Neighbor(const EdgeMaker& maker) {
  // TODO: посмотреть в Саттере где два объект значения
  // No exception safe - но кажется и нельзя сделат безопасным
  // Типы базовые, поэтому таки безопасно
  weight = maker.weight_;
  end = maker.end_;
}

Neighbor EdgeMaker::create() {
  Neighbor edge;
  edge.end = end_;
  edge.weight = weight_;
  return edge;    
}


//namespace graph_persistency {


class RawYieldFunctor {
public:
  RawYieldFunctor() {}
  
  //@Stateless
  pair<int, Neighbors> operator()(const string& arg) const {
    // Передать по ссылке или вернуть значение?
    // Самый быстрый вариант! Нет в цикле есть IO-операции - так что измерения не очень честные.
    // Если вне цикла, то долго. И если передавать в функцию, то тоже долго, но чуть меньше.
    // Нет, кажется не принципиально
    stringstream ss;  // он тяжелый!!! но как его сбросить?
    pair<int, vector<Arrow> > raw_code = io_details::parse_node_data(arg, ss);
    Neighbors n;
    for_each_(Arrow a, raw_code.second) {
      n.push_back(EdgeMaker(a));
    }
    
    //assert(raw_code.size() > 1);
    return make_pair(raw_code.first, n);
  }
  
private:
  // не копируемый похоже
  //stringstream g_ss;  // он тяжелый!!! но как его сбросить?
};

#ifdef USE_TBB
class ApplyFoo {  
  const string* const array; // map only!!
  pair<int, Neighbors>* const out;
  const RawYieldFunctor op;
public:  
  ApplyFoo (const string* a, pair<int, Neighbors>* out_a): array(a), out(out_a) {}  
  void operator()( const blocked_range<int>& r ) const {  
      for (int i = r.begin(), end = r.end(); i != end; ++i ) { 
	// TODO: i - глобальный индек или нет?
	out[i] = op(array[i]);  
      }  
  }  
};
#endif

vector<string> extract_records(const string& filename) 
{
  fstream stream(filename.c_str());
  if (!stream)
    throw std::runtime_error("Error: can't open file");

  vector<string> records;
  // IO operations
  { 
    records.reserve(200);
    string line;  // и не видна, и не в цикле
    while (true) {
      // можно и в буффер читать, но так показалось что проще завершить чтение
      if (!std::getline(stream, line))  
	break;
      records.push_back(line);
    }
  }
  return records;  
}

vector<pair<int, Neighbors> > process_parallel(const vector<string>& records) {
  // нужно реально выделить, резервирование не подходит
  vector<pair<int, Neighbors> > raw_nodes(records.size());  

#ifdef USE_TBB
  // No speed up
  // Linux CPU statistic.
  // http://superuser.com/questions/443406/how-can-i-produce-high-cpu-load-on-a-linux-server
  //
  //
  // https://software.intel.com/sites/products/documentation/doclib/tbb_sa/help/reference/task_scheduler/task_scheduler_init_cls.htm#task_scheduler_init_cls
  task_scheduler_init init(task_scheduler_init::automatic); 
  // Trouble: "видимо у вас проблема в том, что вы ставите 486-е ядро, а оно видит только одно ядро процессора. ставьте 686-е"
  //   uname -r  # посмотреть что за ядро
  //
  {
    parallel_for(
      blocked_range<int>(0,records.size()), 
      ApplyFoo(&records[0], &raw_nodes[0]),
      simple_partitioner());
  }
#endif
  return raw_nodes;
}

vector<pair<int, Neighbors> > process_serial(const vector<string>& records) {
  vector<pair<int, Neighbors> > tmp(records.size());
  if (true) {
    //for (int i = 0; i < 2000; ++i)
    { 
      std::transform(records.begin(), records.end(),
	    tmp.begin(),
	    RawYieldFunctor());
    } 
  }
  return tmp;
}

vector<Neighbors> build_graph(const vector<string>& records) {
  // Не обязательно сортированные, поэтому граф строится отдельно
  vector<pair<int, Neighbors> > raw_nodes = process_parallel(records);
  assert(!raw_nodes.empty());

  // CHECK_POINT
  // Все номера исходящих узвлов уникальны
  set<int> unique_check;
  auto action = [&unique_check] (const pair<int, Neighbors>& val) { 
    unique_check.insert(val.first); 
  };

  std::for_each(begin(raw_nodes), end(raw_nodes), action);
  assert(unique_check.size() == raw_nodes.size());

  // Формирование графа, если узлы уникальны, то можно параллельно записать в рабочий граф.
  // Нужен нулевой индекс.
  vector<Neighbors> graph(raw_nodes.size()+1);  // 0 добавляем как фейковый узел
  typedef std::pair<int, Neighbors> value_type;
  for_each_(const value_type elem, raw_nodes) {
    graph[elem.first] = elem.second;
  }
  return graph;  
}
}   // namespace ..persistency


namespace graph_statistic {
std::ostream& operator<<(std::ostream& os, const NodeInfo& obj)
{
  if (obj.d > 1000)
    os << obj.idx << " : (" << "Inf" << "/" << obj.visited << ")";
  else
    os << obj.idx << " : (" << obj.d << "/" << obj.visited << ")";//<< endl;
  return os;
}

std::ostream& operator<<(std::ostream& os, const vector<NodeInfo>& obj) {
  int i = 0;
  std::for_each(begin(obj), end(obj), [&os, &i] (const NodeInfo& info) {
    os << ' ' << info;
    ++i;
  });
  os << endl;
  return os;
}
}  // space
