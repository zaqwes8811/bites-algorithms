#ifndef REUSE_VIEW_H_
#define REUSE_VIEW_H_

#include <vector>
#include <list>

#include <ostream>
#include <algorithm>

namespace view {
template <typename T>
class Action {
public:
  explicit Action(std::ostream* o_) : o(o_) { }
  void operator()(const T& elem) const {
    *o << elem << " ";
  }
  std::ostream* const o;
}; 
 
template <typename T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& a) 
{
  std::for_each(a.begin(), a.end(), Action<T>(&o));
  o << std::endl;
  return o;
}

template <typename T>
std::ostream& operator<<(std::ostream& o, const std::list<T>& a) 
{
  std::for_each(a.begin(), a.end(), Action<T>(&o));
  o << std::endl;
  return o;
}

/*
template <typename E>
std::ostream& operator<<(std::ostream& o, const std::vector<std::vector<int> >& arr) 
{
  
}
*/


}

#endif  // REUSE_VIEW_H_