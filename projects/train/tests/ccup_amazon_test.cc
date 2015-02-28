

#include <gtest/gtest.h>

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

TEST(CCupAmazon, One) {
  // http://www.careercup.com/question?id=5758712897601536
  // "Bin Packing Problem"
  //
  // More important non calc but optimality of result

  size_t tapeSize = 100;
  vector<int> files({70, 10, 20, 30, 100});

  // V1
  // 1
  sort(files.begin(), files.end(), greater<int>());

  // 2
  size_t b = 0;
  size_t e = files.size() - 1;
  size_t res = 0;

  while(true) {
    if (b >= e)
      break;

    ++res;  // forever
    if (files[b] + files[e] <= tapeSize) {
      ++b;
      --e;
    } else {
      ++b;
    }

    if (b == e)
      ++res;  // even size case
  }

  EXPECT_EQ(3, res);

  // V2 - binary search - it's change something?
}


class StreamGenerator {
public:
  StreamGenerator() :
    ptr(0),
    stream("The angry dog was red. And the cat was also angry.") { }

private:
  size_t ptr;
  string stream;
};

TEST(CCup, Two) {
  // http://www.careercup.com/question?id=5695840750403584
  // FIXME: Stream may be > RAM size!
  //
  // In Java LinkedHashMap - C++ - ?
  //
  // Dyn progr - max len + min weight ?
  //
  // Store to disk some thing?
}

// http://www.careercup.com/question?id=5715661219168256

// http://www.careercup.com/question?id=5676765995335680

// prizoners
// http://www.careercup.com/question?id=5734324101971968

TEST(Amazon, Arrays3) {
  // http://www.careercup.com/question?id=5717493200977920
}

template<typename T>
struct OnQueue {
  OnQueue() : m_buffer(3) { }

  // must be O(1)
  push(const T& val)
  {

  }

  // bad api
  T pop() {

  }

private:
  std::vector<T> m_buffer;
};

TEST(Amazon, TrickyQueue) {
  // http://www.careercup.com/question?id=14781667

  OnQueue<string> q;

  // if array or vector - shift = O(n)
}
