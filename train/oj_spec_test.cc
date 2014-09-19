#include "visuality/view.h"

#include <gtest/gtest.h>

#include <vector>
#include <algorithm>

using namespace view;
using namespace std;

TEST(OJ, GasStantion) {
  // если до конца не дойти, то вернуть -1
  int gas_[] = {4, 1, 3, 6, 1}; vector<int> gas(gas_, gas_ + sizeof gas_ / sizeof gas_[0]);
  int cost_[] = {2, 4, 2, 2, 3}; vector<int> cost(cost_, cost_ + sizeof cost_ / sizeof cost_[0]);

  for (int i = 0; i < gas.size(); ++i) {
    int tank = 0;

    // now rotate
  }

}