#include <gtest/gtest.h>

#include <vector> 
#include <ostream>

// inner
#include "visuality/view.h"

using namespace std;
using view::operator<<;

TEST(W3, Wis) 
{
  int arr[] = {1, 4, 5, 4};
  vector<int> v(arr, arr + sizeof(arr)/sizeof(arr[0]));
  cout << v;
  
  
}

TEST(W3, KnapsackProblem) 
{
  
}