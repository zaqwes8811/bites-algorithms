#ifndef DETAILS_IO_DETAILS_H
#define DETAILS_IO_DETAILS_H

#include <string>
#include <vector>
#include <sstream>
#include <ostream>

namespace io_details {
struct Arrow {
  int end;
  int weight;
  
  Arrow(int end_, int weight_) : end(end_), weight(weight_) { } 
}; 

class Job {
public:
  // TROUBLE: Gotcha 81
  //const   // проблема возникает при работе с алгоритмами - c++11
  int weight;
  //const 
  int length;
  
  // Почему их нужно генерировать?
  //Job(const Job& j) : weight(j.weight), length(j.length) { }
  //Job& operator=(const Job& j) {
  //  Job tmp(j);
  //}
  
  int get_delta() const {
    return weight-length;
  }
  
  float get_ration() const {
    return (1.0f*weight) / length;
  }
  
//private:
  // лучше сделать maker - в конструкторое можно перепутать параметры
  Job(int weight_, int id_) : weight(weight_), length(id_) { } 
};

std::ostream& operator<<(std::ostream& o, const Job& job);

std::pair<int, std::vector<Arrow> > parse_node_data(const std::string& line, std::stringstream& ss);

std::vector<int> extract_records(const std::string& filename);

//
std::vector<Job> get_jobs_fake(const std::string& filename);
std::vector<Job> get_jobs(const std::string& filename);

// http://stackoverflow.com/questions/2436705/defining-for-stl-sort-algorithm-operator-overload-functor-or-standalone-fun
//bool operator<(const Job& j1, const Job& j2);

struct Item {
  Item() : v(0), w(0) {}
  Item(int _v, int _w) : v(_v), w(_w) {}
  
  int v;
  int w;
};

std::pair<int, std::vector<Item> > get_test_items(const std::string& filename);
std::pair<int, std::vector<Item> > get_dyn_items(const std::string& filename);

}
#endif
