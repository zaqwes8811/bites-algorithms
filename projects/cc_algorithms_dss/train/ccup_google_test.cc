
#include <gtest/gtest.h>

#include <vector>
#include <utility>

#include <stdlib.h>

using namespace std;

TEST(CCupGoogle, One) {
  // http://www.careercup.com/question?id=5122639288860672
  // O(1) space
  //
  // Deep math + work with stream
  vector<pair<int, int>> stream({{1,2},{2,2},{3,4},{4,10}});

  int sum = 0;
  size_t size = 0;
  for (auto& e: stream) {
    sum += e.second;
    ++size;
  }

  size_t id = ::rand() % size;

  // FIXME: can I do it for stream? - No.
  auto elem = stream[id];

  int w = elem.second;
}

// http://www.careercup.com/question?id=4925398016065536
