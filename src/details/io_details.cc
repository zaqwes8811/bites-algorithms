#include "details/io_details.h"

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
// istream::sentry example
#include <iostream>     // std::istream, std::cout
#include <string>       // std::string
#include <sstream>      // std::stringstream
#include <locale>       // std::isspace, std::isdigit
#include <vector>
#include <algorithm>

// 3rdparty
#include <boost/lexical_cast.hpp>
//#include <loki/ScopeGuard.h>

namespace try_deserialize {
  using namespace std;
struct Phone {
  std::string digits;
};

/*
 /// Extract objects
  // http://www.cplusplus.com/reference/istream/istream/sentry/
  std::stringstream parseme ("   (555)2326");
  Phone myphone;
  parseme >> myphone;
  std::cout << "digits parsed: " << myphone.digits << '\n';
  return 0;
 */

// custom extractor for objects of type Phone
std::istream& operator>>(std::istream& is, Phone& tel)
{
    std::istream::sentry s(is);
    if (s) while (is.good()) {
      char c = is.get();
      if (std::isspace(c,is.getloc())) break;
      if (std::isdigit(c,is.getloc())) tel.digits += c;
    }
    return is;
}


}  // namespace

namespace io_details {
using namespace std;

pair<int, vector<Item> > get_dyn_items(const string& filename) {
  const char kSplitter = ' ';
  fstream stream(filename.c_str());
  if (!stream)
    throw runtime_error("Error: can't open file");
  
  // W, count items
  int W;
  int count_items;
  string line;
  stringstream ss;
  {
    // Get count
    getline(stream, line);  // разделитель что угодно
    ss.str(line);
    ss.clear();  // можно выполнить через guard
    
    ss >> W;
    ss >> count_items;
  }
  
  cout << count_items << endl;
  
  vector<Item> items;
  items.reserve(count_items);
  while (!stream.fail()) {
    getline(stream, line);  // разделитель что угодно
    ss.str(line);
    ss.clear();
    
    int i = 0;
    ss >> i;
    if (!ss)
      break;
    if (ss.peek() == kSplitter)
      ss.ignore();
    
    // W
    int j = 0;
    ss >> j;
    if (!ss)
      throw invalid_argument("Error: String format is broken.");
    if (ss.peek() == kSplitter)
      ss.ignore();
    
    Item job(i, j);
    items.push_back(job);
  }
  
  if (items.size() != count_items)
    throw runtime_error("Error: brocken file  "+filename);

  return make_pair(W, items);
}

pair<int, vector<Item> > get_test_items(const string& filename) 
{
  int W = 6;
  vector<Item> items;  // (vi, wi)
  items.push_back(Item(0, 0));  // TODO: move
  items.push_back(Item(3, 4));
  items.push_back(Item(2, 3));
  items.push_back(Item(4, 2));
  items.push_back(Item(4, 3));
  return make_pair(W, items);
}

ostream& operator<<(ostream& o, const Job& job) 
{
  o << "w(" << job.weight << ") l(" << job.length << ")";
  return o;  
}

/*
bool operator<(const Job& j1, const Job& j2) 
{
  return j1.weight < j2.weight;
}
//*/

vector<Job> get_jobs_fake(const string&) 
{
  vector<Job> v;
  v.push_back(Job(1, 1));  // 0
  v.push_back(Job(2, 1));  // 1
  v.push_back(Job(1, 3));  // -2
  v.push_back(Job(2, 4));  // -2
  v.push_back(Job(3, 5));  // -2
  return v;
}

vector<Job> get_jobs(const string& filename) {
  stringstream ss;
  const char kSplitter = ' ';
  size_t count = 0;
  
  fstream stream(filename.c_str());
  if (!stream)
    throw runtime_error("Error: can't open file");
  
  string line;
  {
    // Get count
    getline(stream, line);  // разделитель что угодно
    ss.str(line);
    ss.clear();  // можно выполнить через guard
    
    ss >> count;
  }
  
  // Jobs
  vector<Job> jobs;
  jobs.reserve(count);
  while (!stream.fail()) {
    getline(stream, line);  // разделитель что угодно
    ss.str(line);
    ss.clear();
    
    int i = 0;
    ss >> i;
    if (!ss)
      break;
    if (ss.peek() == kSplitter)
      ss.ignore();
    
    // W
    int j = 0;
    ss >> j;
    if (!ss)
      throw invalid_argument("Error: String format is broken.");
    if (ss.peek() == kSplitter)
      ss.ignore();
    
    Job job(i, j);
    jobs.push_back(job);
  }
  assert(count == jobs.size());
  return jobs;
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
  
/*
TEST: 
  string test_line("0\t8,89\t9,90\t\n");
  stringstream ss;
  pair<int, Neighbors> test = parse_node_data(test_line, ss);
  assert(test.second.size() == 2);
 */

// TODO: Возвращаемое значение не всегда копируется?
pair<int, vector<Arrow> > parse_node_data(const string& line, stringstream& ss) 
{
  // 0\t8,89\t...  source sink, weight ... -> 0,8,89 - 
  const char kSplitter = ',';
  
  string line_copy = line;  // TODO: slow
  
  // trim
  // http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
  line_copy.erase(line_copy.find_last_not_of(" \n\r\t")+1);
  
  // http://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string
  // возможно можно исключить
  replace(line_copy.begin(), line_copy.end(), '\t', kSplitter);  // заменяем символы, а не строки

  // можно разбивать на части
  // stringstream - http://habrahabr.ru/post/131977/
  // http://stackoverflow.com/questions/1894886/parsing-a-comma-delimited-stdstring 
  // Try reload
  ss.str(line_copy); 
  ss.clear();
  
  // main node
  int root = 0;
  ss >> root;  // похоже довольно интеллектуальная операция
  if (ss.peek() == kSplitter)
      ss.ignore();

  vector<Arrow> result;  // TODO: to deque
  result.reserve(100);  // защита от лишний аллокаций 
  int max_node_idx = 0;
  while (true) 
  {
    int i = 0;
    bool v = ss >> i;
    if (!v)
      break;
    if (ss.peek() == kSplitter)
      ss.ignore();
    
    // Find max
    if (i > max_node_idx)
      max_node_idx = i;
    
    // W
    int j = 0;
    bool w = ss >> j;
    if (!w)
      throw invalid_argument("Error: String format is broken.");
    if (ss.peek() == kSplitter)
      ss.ignore();
    
    result.push_back(Arrow(i, j));//EdgeMaker().end(i).weight(j));
  }

  //make_pair(max_node_idx
  return make_pair(root, result); // сразу не поставил а gcc не отловил - в итоге дамп памяти
}
  
}  // namespace

