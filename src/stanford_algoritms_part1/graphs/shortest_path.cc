// Type converting:
// "if you use Boost.Tokenizer, why not to replace atoi by boost::lexical_cast?"
//
// String processing: 
//   http://www.codeproject.com/Articles/23198/C-String-Toolkit-StrTk-Tokenizer
//
// STL alg.L:
// http://stackoverflow.com/questions/11343116/rotating-a-vector-array

// C
#include <cassert>

// C++
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <sstream>

// http://www.onlamp.com/pub/a/onlamp/2006/04/06/boostregex.html?page=3
//#include <boost/regex.hpp>  // too hard
//#include "boost/lexical_cast.hpp"  // не то, что нужно - это уже для строк со снятым форматированием

#include <tbb/tbb.h>

#include "shortest_path.h"

using namespace std;
using namespace tbb;

// Named params in ctor:
// http://marcoarena.wordpress.com/2011/05/27/learn-how-to-support-named-parameters-in-cpp/
class EdgeMaker;  // TODO: bad but... don't work anyway
struct Edge {
  Edge() : weight(), end() {}
  int weight;
  int end;  
  Edge(const EdgeMaker& maker);  // реализацию вынести обязательно!
};


class EdgeMaker {
public:
  // выдает предупреждение, если инициализация не в таком порядке как объя. в классе
  EdgeMaker() : weight_(0), end_(0)  { }  
  
  EdgeMaker& end(int end) { end_ = end; return *this;}
  EdgeMaker& weight(int weight) { weight_ = weight; return *this; }
  Edge create();  // лучше, т.к. не нужно лезть в класс, который создается, но запись длиннее
  
private:
  friend class Edge;
  int weight_;
  int end_;
};

Edge::Edge(const EdgeMaker& maker) {
  // TODO: посмотреть в Саттере где два объект значения
  // No exception safe - но кажется и нельзя сделат безопасным
  // Типы базовые, поэтому таки безопасно
  weight = maker.weight_;
  end = maker.end_;
}

Edge EdgeMaker::create() {
    Edge edge;
    edge.end = end_;
    edge.weight = weight_;
    return edge;    
  }


/*
 // Is correct
    string test_line("0\t8,89\t9,90\t\n");
    vector<int> raw_code = process_data_line(test_line, g_ss);
    assert(raw_code.size() == 5);
 */
// TODO: Возвращаемое значение не всегда копируется?
vector<int> process_data_line(const string& line, stringstream& ss) 
{
  // 0\t8,89\t...  source sink, weight ... -> 0,8,89 - 
  vector<int> result;  // TODO: to deque
  result.reserve(100);  // защита от лишний аллокаций 
  const char kSplitter = ',';
  
  string line_copy = line;  // TODO: slow
  
  // trim
  // http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
  line_copy.erase(line_copy.find_last_not_of(" \n\r\t")+1);
  
  // http://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string
  // возможно можно исключить
  //replace(line_copy.begin(), line_copy.end(), '\t', kSplitter);  // заменяем символы, а не строки

  // можно разбивать на части
  // stringstream - http://habrahabr.ru/post/131977/
  // http://stackoverflow.com/questions/1894886/parsing-a-comma-delimited-stdstring 
  // Try reload
  ss.str(line_copy); 
  ss.clear();

  for (int i = 0; ss >> i; ) {
      result.push_back(i);
      if (ss.peek() == kSplitter || ss.peek() == '\t')
	  ss.ignore();
  }
  
  // postcondition
  if (result.size() % 2 == 0)
    throw invalid_argument("Error: String format is broken.");
  
  //out.swap(result);  
  return result; // сразу не поставил а gcc не отловил - в итоге дамп памяти
}

typedef vector<vector<Edge> > RawGraph;

class RawYieldFunctor {
public:
  RawYieldFunctor() {}
  
  vector<int> operator()(const string& arg) {
    // Передать по ссылке или вернуть значение?
    // Самый быстрый вариант! Нет в цикле есть IO-операции - так что измерения не очень честные.
    // Если вне цикла, то долго. И если передавать в функцию, то тоже долго, но чуть меньше.
    // Нет, кажется не принципиально
    stringstream g_ss;  // он тяжелый!!! но как его сбросить?
    vector<int> raw_code = process_data_line(arg, g_ss);
    assert(raw_code.size() > 1);
    return raw_code;
  }
  
private:
  // не копируемый!
  //stringstream g_ss;  // он тяжелый!!! но как его сбросить?
};

vector<string> extract_records(const string& filename) 
{
  fstream stream(filename.c_str());
  if (!stream)
    throw runtime_error("Error: can't open file");

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

int main() 
{
  try {
    vector<string> records = extract_records("../input_data/dijkstraData.txt");
    
    {
      task_scheduler_init init(task_scheduler_init::automatic); 
    }
    for (int i = 0; i < 100; ++i)
    { 
      vector<vector<int> > raw_records(records.size());  // нужно реально выделить, резервирование не подходит
      transform(records.begin(), records.end(),
		raw_records.begin(),
		RawYieldFunctor());
     

    }  
    
  } catch (const exception& e) {
    cout << e.what() << endl;
    //throw;   // don't need it 
  }  
  return 0;
}

      /*
       * //RawGraph gr(records.size());  // TODO: +1?
       //int root = raw_code[0];
      
	// Scatter
	vector<Edge> edges;
	edges.reserve(100);
	
	const size_t kSize = raw_code.size();
	for (size_t i = 1; i < kSize; ++i) {
	  if (0 == (i + 1) % 2) {
	    // Node index
	    Edge edge = EdgeMaker().end(raw_code[i]).weight(raw_code[i+1]);
	    edges.push_back(edge);
	  }
	}
	assert(edges.size() == (kSize -1 ) / 2);
	
	// Connect to graph
       */

