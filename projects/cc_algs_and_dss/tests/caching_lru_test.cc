//http://stackoverflow.com/questions/12561439/c-inmemory-cache-library
//
// http://stxxl.sourceforge.net/

#include <gtest/gtest.h>

#include <functional>

template <typename K, typename V>
class LRUCache {
public:
  typedef std::function<V(K)> Operation;

  explicit LRUCache(Operation op) : m_op(op) { }

  V get(const K& key) {
    return m_op(key);
  }

private:
  Operation m_op;

  // DLL

  // Key-Value store
};

int get(int key) {
  return key;
}

TEST(DSS, Cache) {
  LRUCache<int, int> v(&get);

  v.get(0);
}
