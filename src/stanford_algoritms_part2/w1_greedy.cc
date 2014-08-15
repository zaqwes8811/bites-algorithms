
#include <cassert>

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <string>

// no standart now
//#include <ext/hash_set>
#include <boost/unordered_set.hpp>

// 3rdpary
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>


// App
#include "details/io_details.h"

using namespace std;
using namespace io_details;
using io_details::Job;
//using io_details::JobDecreaseWeightCompator;

namespace {
template <typename T>
ostream& operator<<(ostream& o, const vector<T>& v) {
  for_each(begin(v), end(v), [&o](const T& elem) {
    o << elem << " / ";
  });
  o << endl;
  return o;
}

struct JobDecreaseWeightCompator {
  bool operator() (const Job& j1, const Job& j2) {
    return j1.weight > j2.weight;
  }
};

struct JobDecreaseDeltaCompator {
  bool operator() (const Job& j1, const Job& j2) {
    return j1.get_delta() > j2.get_delta();
  }
};

// Calc completed time 

}


int main() {
  /// Q1
  using io_details::Job;
  
  string w1_1_filename = "../stanford_algoritms_part2/in_data/jobs.txt";
  vector<Job> jobs = io_details::get_jobs_fake(w1_1_filename);
  cout << jobs;
  
  // Overhead, но возможно так лучше, т.к. как потом выстроить по весу не ясно
  sort(jobs.begin(), jobs.end(), JobDecreaseWeightCompator());  // сперва по уменьшения веса
  cout << jobs;
  
  // Нужна стабильность
  stable_sort(jobs.begin(), jobs.end(), JobDecreaseDeltaCompator());  // сперва по уменьшения разницы
  cout << jobs;

  
  /// Q2
  
  /// Q3

  
}