
#ifndef P1_GR_SHORTEST_PATH
#define P1_GR_SHORTEST_PATH

#include <vector>
#include <string>
#include <ostream>

// App
#include "details/io_details.h"

namespace graph_details {
//class Neighbor;


std::vector<std::string> extract_records(const std::string& filename);
  
// Named params in ctor:
// http://marcoarena.wordpress.com/2011/05/27/learn-how-to-support-named-parameters-in-cpp/
class EdgeMaker;  // TODO: bad but... don't work anyway

struct Neighbor {
  Neighbor() : weight(), end() {}
  int weight;
  int end;  
  bool operator==(const Neighbor& that) const { return (that.weight == weight) && (that.end == end); }
  Neighbor(const EdgeMaker& maker);  // реализацию вынести обязательно!
  //http://stackoverflow.com/questions/4421706/operator-overloading
};
typedef std::vector<Neighbor> Neighbors;  // заменить на СОСЕДЕЙ

class EdgeMaker {
public:
  // выдает предупреждение, если инициализация не в таком порядке как объя. в классе
  EdgeMaker() : weight_(0), end_(0)  { }  
  
  // Приведение типа
  EdgeMaker(const io_details::Arrow& a) {
    weight_ = a.weight;
    end_ = a.end;
  }
  
  EdgeMaker& end(int end) { end_ = end; return *this;}
  EdgeMaker& weight(int weight) { weight_ = weight; return *this; }
  Neighbor create();  // лучше, т.к. не нужно лезть в класс, который создается, но запись длиннее
  
private:
  friend class Neighbor;
  EdgeMaker(const EdgeMaker&);
  EdgeMaker operator=(const EdgeMaker&);
  
  int weight_;
  int end_;
};

// Overhead
/*
// Обработчик одного узла для for_each
class OneNodeFunctor {
  //int index;
public:
  OneNodeFunctor();
  
};*/


// Failed
/*
template <typename T>
void print(const T& val) {
  cout << val << endl;
}

//template <typename T>
void print_vec(const vector<int>& val) {
  for_each(val.begin(), val.end(), print);  
}*/
std::vector<Neighbors> build_graph(const std::vector<std::string>& records);

}  // namespace...

namespace graph_statistic {
const int kMaxVal = 1000000;//numeric_limits<int>::max();
  
class NodeInfo {
public:
  NodeInfo() : d(kMaxVal), visited(false) { } 
  int d;
  bool visited;
  size_t idx;
};

std::ostream& operator<<(std::ostream& os, const NodeInfo& obj);
std::ostream& operator<<(std::ostream& os, const std::vector<NodeInfo>& obj);
}

#endif